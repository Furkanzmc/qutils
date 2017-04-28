#pragma once
// Qt
#include <QObject>
// qutils
#include "SqliteManager.h"

namespace zmc
{

/**
 * @brief The SettingsManager class uses a SqliteManager to store the settings. Settings are saved only in TEXT format.
 */
class SettingsManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString databasePath READ getDatabasePath WRITE setDatabasePath NOTIFY databasePathChanged)
    Q_PROPERTY(QString settingsTableName READ getSettingsTableName WRITE setSettingsTableName NOTIFY settingsTableNameChanged)

public:
    explicit SettingsManager(QObject *parent = 0);

    /**
     * @brief Returns the system language in two-letter ISO 639 language code. e.g tr, en
     * @return
     */
    Q_INVOKABLE QString getSystemLanguage() const;

    /**
     * @brief Write a setting to the database. If a setting with the key exists, it is overwritten.
     * @param key
     * @param value
     * @return
     */
    Q_INVOKABLE bool write(const QString &key, const QString &value);

    /**
     * @brief Reads the setting with the given key. If the key doesn't exist, returns an empty string.
     * @param key
     * @return
     */
    Q_INVOKABLE QString read(const QString &key);

    QString getDatabasePath() const;

    /**
     * @brief The database will be reloaded when this property changes.
     * @param databasePath
     */
    void setDatabasePath(const QString &databasePath);

    QString getSettingsTableName() const;
    void setSettingsTableName(const QString &tableName);

private:
    QString m_DatabasePath, m_SettingsTableName;
    zmc::SqliteManager m_SqlManager;
    QSqlDatabase m_Database;

private:
    void createTable();

    /**
     * @brief Opens the database at m_DatabasePath If it is not open. If it is open, does nothing.
     */
    void openDatabase();

    /**
     * @brief Closes and opens the database at m_DatabasePath.
     */
    void restartDatabase();

signals:
    /**
     * @brief This is emitted when a setting is changed.
     * @param settingName
     * @param oldSettingValue This will an empty string if the setting is inserted for the first time.
     * @param newSettingValue
     */
    void settingChanged(QString settingName, QString oldSettingValue, QString newSettingValue);
    void databasePathChanged();
    void settingsTableNameChanged();

    void databaseOpened();
    void databaseClosed();

public slots:
};

}
