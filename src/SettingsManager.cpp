#include "qutils/SettingsManager.h"
// Qt
#include <QLocale>
#include <QStandardPaths>
#include <QList>
// qutils
#include "qutils/Macros.h"

#define COL_SETTING_NAME "setting_name"
#define COL_SETTING_VALUE "setting_value"
#define DATABASE_CHECK() do { if (m_Database.isOpen() == false) { openDatabase(); createTable(); } } while (0)

namespace zmc
{

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
    , m_DatabasePath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/qutils_settings.sqlite")
    , m_SettingsTableName("settings")
    , m_SqlManager()
    , m_Database()
{
}

QString SettingsManager::getSystemLanguage() const
{
    QLocale locale;
    QString name = locale.name();
    return name.left(name.indexOf("_"));
}

bool SettingsManager::write(const QString &key, const QString &value)
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
    newMap[COL_SETTING_VALUE] = value;
    if (exists) {
        const QVariantMap oldMap = existingData.at(0);
        successful = m_SqlManager.updateInTable(m_Database, m_SettingsTableName, newMap, values);

        emit settingChanged(key, oldMap[key].toString(), newMap[key].toString());
    }
    else {
        successful = m_SqlManager.insertIntoTable(m_Database, m_SettingsTableName, newMap);
        if (successful == false) {
            LOG(m_SqlManager.getLastError().query);
        }

        emit settingChanged(key, "", newMap[key].toString());
    }

    return successful;
}

QString SettingsManager::read(const QString &key)
{
    DATABASE_CHECK();

    const QList<SqliteManager::Constraint> values {
        std::make_tuple(COL_SETTING_NAME, key, "AND")
    };

    QString value;
    const QList<QVariantMap> existingData = m_SqlManager.getFromTable(m_Database, m_SettingsTableName, -1, &values);
    const bool exists = existingData.size() > 0;
    if (exists) {
        value = existingData.at(0)[COL_SETTING_VALUE].toString();
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
        SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::TEXT, COL_SETTING_VALUE)
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

}
