#pragma once
// Qt
#include <QObject>
// Local
#include "qutils/SqliteManager.h"
#include "qutils/Macros.h"

#ifdef QUTILS_APP_NAME
#define SETTINGS_DB_FILE_NAME STRINGIFY(QUTILS_APP_NAME) "_settings.sqlite"
#else
#define SETTINGS_DB_FILE_NAME "qutils_settings.sqlite"
#endif // QUTILS_APP_NAME

namespace zmc
{

/**
 * @brief The SettingsManager class uses a SqliteManager to store the settings. Settings are saved in QVariant format.
 * When a setting is changed the settingChanged signal is emitted and this signal is emitted in all of the instances.
 */
class SettingsManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString databasePath READ getDatabaseName WRITE setDatabaseName NOTIFY databasePathChanged)
    Q_PROPERTY(QString settingsTableName READ getSettingsTableName WRITE setSettingsTableName NOTIFY settingsTableNameChanged)

public:
    explicit SettingsManager(QString databaseName = SETTINGS_DB_FILE_NAME, QString tableName = "settings", QObject *parent = 0);
    ~SettingsManager();

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
    Q_INVOKABLE bool write(const QString &key, const QVariant &value);

    /**
     * @brief Reads the setting with the given key. If the key doesn't exist, returns an empty string.
     * @param key
     * @return
     */
    Q_INVOKABLE QVariant read(const QString &key);

    /**
     * @brief Removes the given key from the settings.
     * @param key
     * @return bool Returns true If the key was removed, false if it wasn't.
     */
    Q_INVOKABLE bool remove(const QString &key);

    /**
     * @brief Checks If the given key exists in the cache.
     * @param key
     * @return bool Returns true If the key exists.
     */
    Q_INVOKABLE bool exists(const QString &key);

    /**
     * @brief Clears the whole settings.
     * @return bool
     */
    Q_INVOKABLE bool clear();

    /**
     * @brief Returns the list of keys in the settings.
     * @return QStringList
     */
    Q_INVOKABLE QStringList getKeys();

    QString getDatabaseName() const;

    /**
     * @brief The database will be reloaded when this property changes. This is the database file name only. Cache is alwasy created at the writable location.
     * @param databasePath
     */
    void setDatabaseName(const QString &databaseName);

    QString getSettingsTableName() const;
    void setSettingsTableName(const QString &tableName);

private:
    static QList<SettingsManager *> m_Instances;
    static bool m_IsTableCreated;

    const int m_InstanceIndex;
    QString m_DatabaseName, m_SettingsTableName;
    zmc::SqliteManager m_SqlManager;

private:
    /**
     * @brief Creates the table in the database. For this operation, the database must be open.
     * @return True If the operation suceeds.
     */
    bool createTable();

    /**
     * @brief Opens the database at m_DatabasePath If it is not open. If it is open, does nothing.
     */
    void openDatabase();

    /**
     * @brief Closes and opens the database at m_DatabasePath.
     */
    void restartDatabase();

    static void emitSettingChangedInAllInstances(const QString &settingName, const QVariant &oldSettingValue, const QVariant &newSettingValue);
    static void emitClearedSignals();

signals:
    /**
     * @brief This is emitted when a setting is changed. This signal is emitted in all of the SettingsManager instances when a setting is changed.
     * @param settingName
     * @param oldSettingValue This will an empty string if the setting is inserted for the first time.
     * @param newSettingValue
     */
    void settingChanged(QString settingName, QVariant oldSettingValue, QVariant newSettingValue);
    void databasePathChanged();
    void settingsTableNameChanged();

    void databaseOpened();
    void databaseClosed();
    void cleared();

public slots:
};

}
