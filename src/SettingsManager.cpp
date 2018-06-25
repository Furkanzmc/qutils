#include "qutils/SettingsManager.h"
// Qt
#include <QLocale>
#include <QStandardPaths>
#include <QList>
#include <QDir>
// Local
#include "qutils/JsonUtils.h"
#include "qutils/Macros.h"

#define COL_SETTING_NAME "setting_name"
#define COL_SETTING_VALUE "setting_value"
#define COL_SETTING_TYPE "setting_type"
#define DATABASE_CHECK() do { if (database.isOpen() == false) { LOG_WARNING("Database is not open!"); return false; } } while (0)

namespace zmc
{

QMap<int, SettingsManager *> SettingsManager::m_Instances = QMap<int, SettingsManager *>();

SettingsManager::SettingsManager(QString databaseName, QString tableName, QObject *parent)
    : QObject(parent)
    , m_InstanceIndex(m_Instances.size())
    , m_DatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + databaseName)
    , m_TableName(tableName)
    , m_SqlManager()
    , m_IsTableCreated(false)
{
    m_Instances.insert(m_InstanceIndex, this);

    // Create the app data location folder if it doesn't exist.
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (dir.exists() == false) {
        dir.mkpath(dir.path());
    }

    openDatabase();
    createTable();
}

SettingsManager::~SettingsManager()
{
    m_Instances.remove(m_InstanceIndex);
    if (m_Instances.size() == 0) {
        LOG("Removing database.");
        m_SqlManager.removeDatabase(m_DatabaseName);
    }
}

QString SettingsManager::getSystemLanguage() const
{
    QLocale locale;
    QString name = locale.name();
    return name.left(name.indexOf("_"));
}

bool SettingsManager::write(const QString &key, const QVariant &value)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK();

    bool successful = false;
    const QList<SqliteManager::Constraint> values {
        std::make_tuple(COL_SETTING_NAME, key, "AND")
    };

    const QList<QMap<QString, QVariant>> existingData = m_SqlManager.getFromTable(database, m_TableName, -1, &values);
    const bool exists = existingData.size() > 0;
    QMap<QString, QVariant> newMap;
    newMap[COL_SETTING_NAME] = key;
    const int dataTypeID = value.type();
    if (dataTypeID == QVariant::Type::List || dataTypeID == QVariant::Type::StringList) {
        newMap[COL_SETTING_VALUE] = JsonUtils::toJsonString(value.toList());
    }
    else if (dataTypeID == QVariant::Type::Map) {
        newMap[COL_SETTING_VALUE] = JsonUtils::toJsonString(value.toMap());
    }
    else {
        newMap[COL_SETTING_VALUE] = value;
    }

    newMap[COL_SETTING_TYPE] = QVariant::fromValue<int>(value.type());

    if (exists) {
        const QVariantMap oldMap = existingData.at(0);
        successful = m_SqlManager.updateInTable(database, m_TableName, newMap, values);
        QVariant oldValue = oldMap[COL_SETTING_VALUE];
        oldValue.convert(oldMap[COL_SETTING_TYPE].toInt());

        emitSettingChangedInAllInstances(key, oldValue, value, m_TableName, m_DatabaseName);
    }
    else {
        successful = m_SqlManager.insertIntoTable(database, m_TableName, newMap);
        emitSettingChangedInAllInstances(key, "", value, m_TableName, m_DatabaseName);
    }

    return successful;
}

QVariant SettingsManager::read(const QString &key)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> values {
        std::make_tuple(COL_SETTING_NAME, key, "AND")
    };

    QVariant value;
    const QList<QMap<QString, QVariant>> existingData = m_SqlManager.getFromTable(database, m_TableName, -1, &values);
    const bool exists = existingData.size() > 0;
    if (exists) {
        value = existingData.at(0)[COL_SETTING_VALUE];
        const int dataTypeID = existingData.at(0)[COL_SETTING_TYPE].toInt();
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

bool SettingsManager::remove(const QString &key)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_SETTING_NAME, key, "AND")
    };

    return m_SqlManager.deleteInTable(database, m_TableName, constraints);
}

bool SettingsManager::exists(const QString &key)
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> constraints {
        std::make_tuple(COL_SETTING_NAME, key, "AND")
    };

    return m_SqlManager.exists(database, m_TableName, constraints);
}

bool SettingsManager::clear()
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    DATABASE_CHECK();

    bool successful = m_SqlManager.clearTable(database, m_TableName);
    if (successful) {
        emitClearedSignals();
    }

    return successful;
}

QStringList SettingsManager::getKeys()
{
    QStringList keys;
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    if (database.isOpen() == false) {
        LOG_WARNING("Database is not open!");
        return keys;
    }

    const QList<QMap<QString, QVariant>> existingData = m_SqlManager.getFromTable(database, m_TableName);

    for (const QMap<QString, QVariant> &val : existingData) {
        keys.append(val.value(COL_SETTING_NAME).toString());
    }

    return keys;
}

QString SettingsManager::getDatabaseName() const
{
    return m_DatabaseName;
}

void SettingsManager::setDatabaseName(const QString &databaseName)
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

QString SettingsManager::getTableName() const
{
    return m_TableName;
}

void SettingsManager::setTableName(const QString &tableName)
{
    const bool changed = m_TableName != tableName;
    if (changed) {
        m_TableName = tableName;

        createTable();
        emit tableNameChanged();
    }
}

bool SettingsManager::createTable()
{
    if (m_IsTableCreated == false) {
        QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
        DATABASE_CHECK();

        QList<SqliteManager::ColumnDefinition> columns {
            SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::TEXT, COL_SETTING_NAME),
            SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::BLOB, COL_SETTING_VALUE),
            SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::INTEGER, COL_SETTING_TYPE)
        };

        m_IsTableCreated = m_SqlManager.createTable(database, columns, m_TableName);
    }
    else {
        LOG("Table, " << m_TableName << ", was already created.");
    }

    return m_IsTableCreated;
}

void SettingsManager::openDatabase()
{
    QSqlDatabase database = m_SqlManager.openDatabase(m_DatabaseName);
    if (database.isOpen() == false) {
        database = m_SqlManager.openDatabase(m_DatabaseName);
        if (database.isOpen()) {
            emit databaseOpened();
        }
    }
}

void SettingsManager::restartDatabase()
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

void SettingsManager::emitSettingChangedInAllInstances(const QString &settingName, const QVariant &oldSettingValue, const QVariant &newSettingValue,
    const QString &tableName, const QString &databaseName)
{
    if (oldSettingValue != newSettingValue) {
        auto begin = m_Instances.begin();
        auto end = m_Instances.end();
        for (auto it = begin; it != end; it++) {
            SettingsManager *instance = it.value();
            if (instance && instance->getTableName() == tableName && instance->getDatabaseName() == databaseName) {
                emit instance->settingChanged(settingName, oldSettingValue, newSettingValue);
            }
        }
    }
}

void SettingsManager::emitClearedSignals()
{
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        SettingsManager *instance = it.value();
        if (instance) {
            emit instance->cleared();
        }
    }
}

}
