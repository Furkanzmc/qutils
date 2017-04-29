#include "qutils/SettingsManager.h"
// Qt
#include <QLocale>
#include <QStandardPaths>
#include <QList>
// qutils
#include "qutils/Macros.h"

#define COL_SETTING_NAME "setting_name"
#define COL_SETTING_VALUE "setting_value"
#define COL_SETTING_TYPE "setting_type"
#define DATABASE_CHECK() do { if (m_Database.isOpen() == false) { openDatabase(); createTable(); } } while (0)

namespace zmc
{

QList<SettingsManager *> SettingsManager::m_Instances = QList<SettingsManager *>();
int SettingsManager::m_InstanceLastIndex = 0;

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
    , m_InstanceIndex(m_InstanceLastIndex)
    , m_DatabasePath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/qutils_settings.sqlite")
    , m_SettingsTableName("settings")
    , m_SqlManager()
    , m_Database()
{
    m_Instances.append(this);
    m_InstanceLastIndex++;
}

SettingsManager::~SettingsManager()
{
    m_Instances[m_InstanceIndex] = nullptr;
}

QString SettingsManager::getSystemLanguage() const
{
    QLocale locale;
    QString name = locale.name();
    return name.left(name.indexOf("_"));
}

bool SettingsManager::write(const QString &key, const QVariant &value)
{
    DATABASE_CHECK();

    bool successful = false;
    const QList<SqliteManager::Constraint> values {
        std::make_tuple(COL_SETTING_NAME, key, "AND")
    };

    const QList<QVariantMap> existingData = m_SqlManager.getFromTable(m_Database, m_SettingsTableName, -1, &values);
    const bool exists = existingData.size() > 0;
    QVariantMap newMap;
    newMap[COL_SETTING_NAME] = key;
    newMap[COL_SETTING_VALUE] = value.toByteArray();
    newMap[COL_SETTING_TYPE] = QVariant::fromValue<int>(value.type());

    if (exists) {
        const QVariantMap oldMap = existingData.at(0);
        successful = m_SqlManager.updateInTable(m_Database, m_SettingsTableName, newMap, values);

        emitSettingChangedInAllInstances(key, oldMap[COL_SETTING_VALUE].toString(), newMap[COL_SETTING_VALUE].toString());
    }
    else {
        successful = m_SqlManager.insertIntoTable(m_Database, m_SettingsTableName, newMap);

        emitSettingChangedInAllInstances(key, "", newMap[COL_SETTING_VALUE].toString());
    }

    return successful;
}

QVariant SettingsManager::read(const QString &key)
{
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> values {
        std::make_tuple(COL_SETTING_NAME, key, "AND")
    };

    QVariant value;
    const QList<QVariantMap> existingData = m_SqlManager.getFromTable(m_Database, m_SettingsTableName, -1, &values);
    const bool exists = existingData.size() > 0;
    if (exists) {
        value = existingData.at(0)[COL_SETTING_VALUE];
        value.convert(existingData.at(0)[COL_SETTING_TYPE].toInt());
    }

    return value;
}

QString SettingsManager::getDatabasePath() const
{
    return m_DatabasePath;
}

void SettingsManager::setDatabasePath(const QString &databasePath)
{
    m_DatabasePath = databasePath;
    emit databasePathChanged();

    restartDatabase();
    createTable();
}

QString SettingsManager::getSettingsTableName() const
{
    return m_SettingsTableName;
}

void SettingsManager::setSettingsTableName(const QString &tableName)
{
    m_SettingsTableName = tableName;
    emit settingsTableNameChanged();

    restartDatabase();
    createTable();
}

void SettingsManager::createTable()
{
    DATABASE_CHECK();

    QList<SqliteManager::ColumnDefinition> columns {
        SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::TEXT, COL_SETTING_NAME),
        SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::BLOB, COL_SETTING_VALUE),
        SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::INTEGER, COL_SETTING_TYPE)
    };

    m_SqlManager.createTable(m_Database, columns, m_SettingsTableName);
}

void SettingsManager::openDatabase()
{
    if (m_Database.isOpen() == false) {
        m_Database = m_SqlManager.openDatabase(m_DatabasePath);
        emit databaseOpened();
    }
}

void SettingsManager::restartDatabase()
{
    if (m_Database.isOpen()) {
        m_SqlManager.closeDatabase(m_Database);
        emit databaseClosed();
    }

    m_Database = m_SqlManager.openDatabase(m_DatabasePath);
    emit databaseOpened();
}

void SettingsManager::emitSettingChangedInAllInstances(const QString &settingName, const QString &oldSettingValue, const QString &newSettingValue)
{
    if (oldSettingValue != newSettingValue) {
        for (SettingsManager *man : m_Instances) {
            if (man) {
                emitSettingChanged(settingName, oldSettingValue, newSettingValue);
            }
        }
    }
}

void SettingsManager::emitSettingChanged(const QString &settingName, const QString &oldSettingValue, const QString &newSettingValue)
{
    emit settingChanged(settingName, oldSettingValue, newSettingValue);
}

}
