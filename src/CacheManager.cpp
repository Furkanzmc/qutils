#include "qutils/CacheManager.h"
#include "qutils/JsonUtils.h"
// Qt
#include <QLocale>
#include <QStandardPaths>
#include <QList>
#include <QDir>
// qutils
#include "qutils/Macros.h"

#define COL_CACHE_NAME "cache_name"
#define COL_CACHE_VALUE "cache_value"
#define COL_CACHE_TYPE "cache_type"
#define DATABASE_CHECK() do { if (database.isOpen() == false) { LOG_WARNING("Database is not open!"); return false; } } while (0)

namespace zmc
{

QList<CacheManager *> CacheManager::m_Instances = QList<CacheManager *>();
bool CacheManager::m_IsTableCreated = false;

CacheManager::CacheManager(QString databaseName, QString tableName, QObject *parent)
    : QObject(parent)
    , m_InstanceIndex(m_Instances.size())
    , m_DatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + databaseName)
    , m_CacheTableName(tableName)
    , m_SqlManager()
{
    m_Instances.append(this);
    // Create the app data location folder if it doesn't exist.
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (dir.exists() == false) {
        dir.mkpath(dir.path());
    }

    openDatabase();
    createTable();
}

CacheManager::~CacheManager()
{
    m_Instances[m_InstanceIndex] = nullptr;
    const int count = std::count_if(m_Instances.begin(), m_Instances.end(), [](CacheManager * instance) {
        return instance != nullptr;
    });

    if (count == 0) {
        LOG("Removing database.");
        m_SqlManager.removeDatabase(m_DatabaseName);
    }
}

bool CacheManager::write(const QString &key, const QVariant &value)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK();

    bool successful = false;
    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };

    const QList<QMap<QString, QVariant>> existingData = m_SqlManager.getFromTable(database, m_CacheTableName, -1, &constraints);
    const bool exists = existingData.size() > 0;
    const int dataTypeID = value.type();

    QMap<QString, QVariant> newMap;
    newMap[COL_CACHE_NAME] = key;

    if (dataTypeID == QVariant::Type::List || dataTypeID == QVariant::Type::StringList) {
        newMap[COL_CACHE_VALUE] = JsonUtils::toJsonString(value.toList());
    }
    else if (dataTypeID == QVariant::Type::Map) {
        newMap[COL_CACHE_VALUE] = JsonUtils::toJsonString(value.toMap());
    }
    else {
        newMap[COL_CACHE_VALUE] = value;
    }

    newMap[COL_CACHE_TYPE] = QVariant::fromValue<int>(value.type());

    if (exists) {
        const QVariantMap oldMap = existingData.at(0);
        successful = m_SqlManager.updateInTable(database, m_CacheTableName, newMap, constraints);
        QVariant oldValue = oldMap[COL_CACHE_VALUE];
        oldValue.convert(oldMap[COL_CACHE_VALUE].toInt());

        emitCacheChangedInAllInstances(key, oldMap[COL_CACHE_VALUE].toString(), newMap[COL_CACHE_VALUE].toString());
    }
    else {
        successful = m_SqlManager.insertIntoTable(database, m_CacheTableName, newMap);
        emitCacheChangedInAllInstances(key, "", value);
    }

    return successful;
}

QVariant CacheManager::read(const QString &key)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> values {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };


    QVariant value;
    const QList<QMap<QString, QVariant>> existingData = m_SqlManager.getFromTable(database, m_CacheTableName, -1, &values);
    const bool exists = existingData.size() > 0;
    if (exists) {
        value = existingData.at(0)[COL_CACHE_VALUE];
        const int dataTypeID = existingData.at(0)[COL_CACHE_TYPE].toInt();
        if (dataTypeID == QVariant::Type::List || dataTypeID == QVariant::Type::StringList) {
            value = JsonUtils::toVariantList(value.toString());
        }
        else if (dataTypeID == QVariant::Type::Map) {
            value = JsonUtils::toVariantMap(value.toString());
        }
        else {
            value.convert(dataTypeID);
        }
    }

    return value;
}

bool CacheManager::remove(const QString &key)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };

    return m_SqlManager.deleteInTable(database, m_CacheTableName, constraints);
}

bool CacheManager::exists(const QString &key)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };

    return m_SqlManager.exists(database, m_CacheTableName, constraints);
}

bool CacheManager::clear()
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK();

    emit cleared();
    return m_SqlManager.clearTable(database, m_CacheTableName);
}

QString CacheManager::getDatabaseName() const
{
    return m_DatabaseName;
}

void CacheManager::setDatabaseName(const QString &databaseName)
{
    if (QDir::isAbsolutePath(databaseName)) {
        LOG_ERROR("Absolute path is given! Database name is just the file name.");
        return;
    }

    m_DatabaseName = databaseName;
    emit databaseNameChanged();

    restartDatabase();
    createTable();
}

QString CacheManager::getCacheTableName() const
{
    return m_CacheTableName;
}

void CacheManager::setCacheTableName(const QString &tableName)
{
    m_CacheTableName = tableName;
    emit cacheTableNameChanged();

    restartDatabase();
    createTable();
}

QString CacheManager::getWritableLocation() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

bool CacheManager::createTable()
{
    if (m_IsTableCreated == false) {
        QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
        DATABASE_CHECK();

        QList<SqliteManager::ColumnDefinition> columns {
            SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::TEXT, COL_CACHE_NAME),
            SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::BLOB, COL_CACHE_VALUE),
            SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::INTEGER, COL_CACHE_TYPE)
        };

        m_IsTableCreated = m_SqlManager.createTable(database, columns, m_CacheTableName);
    }

    return m_IsTableCreated;
}

void CacheManager::openDatabase()
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    if (database.isOpen() == false) {
        database = m_SqlManager.openDatabase(m_DatabaseName);
        if (database.isOpen()) {
            emit databaseOpened();
        }
    }
}

void CacheManager::restartDatabase()
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    if (database.isOpen()) {
        m_SqlManager.closeDatabase(database);
        emit databaseClosed();
    }

    database = m_SqlManager.openDatabase(m_DatabaseName);
    if (database.isOpen()) {
        emit databaseOpened();
    }
}

void CacheManager::emitCacheChangedInAllInstances(const QString &cacheName, const QVariant &oldCachedValue, const QVariant &newCachedValue)
{
    if (oldCachedValue != newCachedValue) {
        const int currentCount = m_Instances.count();
        for (int index = 0; index < currentCount; index++) {
            CacheManager *instance = m_Instances.at(index);
            if (instance) {
                emit instance->cacheChanged(cacheName, oldCachedValue, newCachedValue);
            }
        }
    }
}
}
