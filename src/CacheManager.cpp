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
#define DATABASE_CHECK() do { if (m_Database.isOpen() == false) { openDatabase(); createTable(); } } while (0)

namespace zmc
{

QList<CacheManager *> CacheManager::m_Instances = QList<CacheManager *>();
QSqlDatabase CacheManager::m_Database = QSqlDatabase();

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
}

CacheManager::~CacheManager()
{
    m_Instances[m_InstanceIndex] = nullptr;
}

bool CacheManager::write(const QString &key, const QVariant &value)
{
    DATABASE_CHECK();

    bool successful = false;
    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };

    const QList<QMap<QString, QVariant>> existingData = m_SqlManager.getFromTable(m_Database, m_CacheTableName, -1, &constraints);
    const bool exists = existingData.size() > 0;
    QMap<QString, QVariant> newMap;
    newMap[COL_CACHE_NAME] = key;
    const int dataTypeID = value.type();
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
        successful = m_SqlManager.updateInTable(m_Database, m_CacheTableName, newMap, constraints);
        QVariant oldValue = oldMap[COL_CACHE_VALUE];
        oldValue.convert(oldMap[COL_CACHE_VALUE].toInt());

        emitCacheChangedInAllInstances(key, oldMap[COL_CACHE_VALUE].toString(), newMap[COL_CACHE_VALUE].toString());
    }
    else {
        successful = m_SqlManager.insertIntoTable(m_Database, m_CacheTableName, newMap);
        emitCacheChangedInAllInstances(key, "", value);
    }

    return successful;
}

QVariant CacheManager::read(const QString &key)
{
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> values {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };

    QVariant value;
    const QList<QMap<QString, QVariant>> existingData = m_SqlManager.getFromTable(m_Database, m_CacheTableName, -1, &values);
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
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };

    return m_SqlManager.deleteInTable(m_Database, m_CacheTableName, constraints);
}

bool CacheManager::exists(const QString &key)
{
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_CACHE_NAME, key, "AND")
    };

    return m_SqlManager.exists(m_Database, m_CacheTableName, constraints);
}

bool CacheManager::clear()
{
    DATABASE_CHECK();

    emit cleared();
    return m_SqlManager.clearTable(m_Database, m_CacheTableName);
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

void CacheManager::createTable()
{
    DATABASE_CHECK();

    QList<SqliteManager::ColumnDefinition> columns {
        SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::TEXT, COL_CACHE_NAME),
        SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::BLOB, COL_CACHE_VALUE),
        SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::INTEGER, COL_CACHE_TYPE)
    };

    m_SqlManager.createTable(m_Database, columns, m_CacheTableName);
}

void CacheManager::openDatabase()
{
    if (m_Database.isOpen() == false) {
        m_Database = m_SqlManager.openDatabase(m_DatabaseName);
        emit databaseOpened();
    }
}

void CacheManager::restartDatabase()
{
    if (m_Database.isOpen()) {
        m_SqlManager.closeDatabase(m_Database);
        emit databaseClosed();
    }

    m_Database = m_SqlManager.openDatabase(m_DatabaseName);
    emit databaseOpened();
}

void CacheManager::emitCacheChangedInAllInstances(const QString &settingName, const QVariant &oldSettingValue, const QVariant &newCachedValue)
{
    if (oldSettingValue != newCachedValue) {
        for (CacheManager *man : m_Instances) {
            if (man) {
                man->emitCacheChanged(settingName, oldSettingValue, newCachedValue);
            }
        }
    }
}

void CacheManager::emitCacheChanged(const QString &cacheName, const QVariant &oldCachedValue, const QVariant &newCachedValue)
{
    emit cacheChanged(cacheName, oldCachedValue, newCachedValue);
}

}
