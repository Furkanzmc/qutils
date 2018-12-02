#include "qutils/CacheManager.h"
// Qt
#include <QStandardPaths>
#include <QLocale>
#include <QList>
#include <QDir>
// Local
#include "qutils/JsonUtils.h"
#include "qutils/Macros.h"

#define COL_CACHE_NAME "cache_name"
#define COL_CACHE_VALUE "cache_value"
#define COL_CACHE_TYPE "cache_type"
#define DATABASE_CHECK(database) do { if (database.isOpen() == false) { LOG_WARNING("Database is not open!"); return false; } } while (0)

namespace zmc
{

QMap<int, CacheManager *> CacheManager::m_Instances = QMap<int, CacheManager *>();
unsigned int CacheManager::m_NextInstanceID = 0;

CacheManager::CacheManager(const QString &databaseName, const QString &tableName, QObject *parent)
    : QObject(parent)
    , m_InstanceIndex(m_NextInstanceID)
    , m_DatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + databaseName)
    , m_TableName(tableName)
    , m_IsTableCreated(false)
{
    m_NextInstanceID++;
    m_Instances.insert(m_InstanceIndex, this);
    // Create the app data location folder if it doesn't exist.
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!dir.exists()) {
        dir.mkpath(dir.path());
    }

    dir.setPath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if (!dir.exists()) {
        dir.mkpath(dir.path());
    }

    openDatabase();
    createTable();
}

CacheManager::~CacheManager()
{
    m_Instances.remove(m_InstanceIndex);

    if (m_Instances.isEmpty()) {
        m_SqlManager.removeDatabase(m_DatabaseName);
    }
}

bool CacheManager::write(const QString &key, const QVariant &value)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK(database);

    bool successful = false;
    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };

    const QList<QMap<QString, QVariant>> existingData = m_SqlManager.getFromTable(database, m_TableName, -1, &constraints);
    const bool exists = !existingData.isEmpty();
    const QVariant::Type dataTypeID = value.type();

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

    newMap[COL_CACHE_TYPE] = QVariant::fromValue<int>(static_cast<int>(value.type()));

    if (exists) {
        const QVariantMap &oldMap = existingData.at(0);
        successful = m_SqlManager.updateInTable(database, m_TableName, newMap, constraints);
        QVariant oldValue = oldMap[COL_CACHE_VALUE];
        oldValue.convert(oldMap[COL_CACHE_VALUE].toInt());

        emitCacheChangedInAllInstances(key, oldMap[COL_CACHE_VALUE].toString(), newMap[COL_CACHE_VALUE].toString(), m_DatabaseName, m_TableName);
    }
    else {
        successful = m_SqlManager.insertIntoTable(database, m_TableName, newMap);
        emitCacheChangedInAllInstances(key, "", value, m_DatabaseName, m_TableName);
    }

    return successful;
}

QVariant CacheManager::read(const QString &key)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK(database);

    const QList<SqliteManager::Constraint> values {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };


    QVariant value;
    const QList<QMap<QString, QVariant>> existingData = m_SqlManager.getFromTable(database, m_TableName, -1, &values);
    const bool exists = !existingData.isEmpty();
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
    DATABASE_CHECK(database);

    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };

    return m_SqlManager.deleteInTable(database, m_TableName, constraints);
}

bool CacheManager::exists(const QString &key)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK(database);

    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };

    return m_SqlManager.exists(database, m_TableName, constraints);
}

bool CacheManager::clear()
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK(database);

    const bool success = m_SqlManager.clearTable(database, m_TableName);
    if (success) {
        emit cleared();
    }

    return success;
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

    const bool changed = m_DatabaseName != databaseName;
    if (changed) {
        m_IsTableCreated = false;
        m_DatabaseName = databaseName;

        restartDatabase();
        createTable();
        emit databaseNameChanged();
    }
}

QString CacheManager::getTableName() const
{
    return m_TableName;
}

void CacheManager::setTableName(const QString &tableName)
{
    const bool changed = m_TableName != tableName;
    if (changed) {
        m_IsTableCreated = false;
        m_TableName = tableName;

        createTable();
        emit tableNameChanged();
    }
}

QString CacheManager::getWritableLocation() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

bool CacheManager::createTable()
{
    if (!m_IsTableCreated) {
        QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
        DATABASE_CHECK(database);

        QList<SqliteManager::ColumnDefinition> columns {
            SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::TEXT, COL_CACHE_NAME),
            SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::BLOB, COL_CACHE_VALUE),
            SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::INTEGER, COL_CACHE_TYPE)
        };

        m_IsTableCreated = m_SqlManager.createTable(database, columns, m_TableName);
    }
    else {
        LOG("Table, " << m_TableName << ", was already created.");
    }

    return m_IsTableCreated;
}

void CacheManager::openDatabase()
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    if (!database.isOpen()) {
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

void CacheManager::emitCacheChangedInAllInstances(const QString &cacheName, const QVariant &oldCachedValue, const QVariant &newCachedValue,
    const QString &databaseName, const QString &tableName)
{
    if (oldCachedValue != newCachedValue) {
        auto begin = m_Instances.begin();
        auto end = m_Instances.end();
        for (auto it = begin; it != end; it++) {
            CacheManager *instance = it.value();
            if (instance && instance->getDatabaseName() == databaseName && instance->getTableName() == tableName) {
                QMetaObject::invokeMethod(instance, std::bind(&CacheManager::cacheChanged, instance, cacheName, oldCachedValue, newCachedValue), Qt::QueuedConnection);
            }
        }
    }
}
}
