#pragma once
// Qt
#include <QObject>
// Local
#include "qutils/SqliteManager.h"
#include "qutils/Macros.h"

#ifndef QUTILS_SETTINGS_DB_FILE_NAME
    #ifdef QUTILS_APP_NAME
        #define SETTINGS_DB_FILE_NAME STRINGIFY(QUTILS_APP_NAME) "_settings.sqlite"
    #else
        #define SETTINGS_DB_FILE_NAME "qutils_settings.sqlite"
    #endif // QUTILS_APP_NAME
#else
    #define SETTINGS_DB_FILE_NAME STRINGIFY(QUTILS_SETTINGS_DB_FILE_NAME)
#endif // QUTILS_SETTINGS_DB_FILE_NAME

namespace zmc
{

/*!
 * \brief The SettingsManager class uses a SqliteManager to store the settings. Settings are saved in QVariant format.
 * When a setting is changed the settingChanged signal is emitted and this signal is emitted in all of the instances.
 */
class SettingsManager : public QObject
{
    Q_OBJECT Q_PROPERTY(QString databasePath READ getDatabaseName WRITE setDatabaseName NOTIFY
        databaseNameChanged)
    Q_PROPERTY(QString settingsTableName READ getTableName WRITE setTableName NOTIFY tableNameChanged)

public:
    explicit SettingsManager(const QString &databaseName = SETTINGS_DB_FILE_NAME,
        const QString &tableName = "settings", QObject *parent = 0);
    ~SettingsManager();

    /*!
     * \brief Returns the system language in two-letter ISO 639 language code. e.g tr, en
     * \return
     */
    Q_INVOKABLE QString getSystemLanguage() const;

    /*!
     * \brief Write a setting to the database. If a setting with the key exists, it is overwritten.
     * \param key
     * \param value
     * \return
     */
    Q_INVOKABLE bool write(const QString &key, const QVariant &value);

    /*!
     * \brief Reads the setting with the given key. If the key doesn't exist, returns an empty string.
     * \param key
     * \return
     */
    Q_INVOKABLE QVariant read(const QString &key);

    /*!
     * \brief Removes the given key from the settings.
     * \param key
     * \return bool Returns true If the key was removed, false if it wasn't.
     */
    Q_INVOKABLE bool remove(const QString &key);

    /*!
     * \brief Checks If the given key exists in the cache.
     * \param key
     * \return bool Returns true If the key exists.
     */
    Q_INVOKABLE bool exists(const QString &key);

    /*!
     * \brief Clears the whole settings.
     * \return bool
     */
    Q_INVOKABLE bool clear();

    /*!
     * \brief Returns the list of keys in the settings.
     * \return QStringList
     */
    Q_INVOKABLE QStringList getKeys();

    /*!
     * \brief Returns the database name currently in use.
     * \return QString
     */
    QString getDatabaseName() const;

    /*!
     * \brief The database will be reloaded when this property changes. This is the database file name only. Cache is alwasy created at the writable location.
     * \param databasePath
     */
    void setDatabaseName(const QString &databaseName);

    /*!
     * \brief Returns the table name currently in use.
     * \return QString
     */
    QString getTableName() const;

    /*!
     * \brief Sets the table name to \a tableName and refreshes the database.
     * \param tableName
     */
    void setTableName(const QString &tableName);

private:

    /*!
     * \variable static QMap<int, CacheManager *> m_Instances
     * \brief This variable is used to keep track of all the instances so that we can send signals to all of them.
     */
    static QMap<int, SettingsManager *> m_Instances;

    /*!
     * \variable static unsigned int m_NextInstanceID
     * \brief Used to prevent key clashes in m_Instances.
     */
    static unsigned int m_NextInstanceID;

    /*!
     * \variable const int m_InstanceIndex
     * \brief This variable is initilized in the constructor to the size value of m_Instances.
     *
     * This is used to identify the current instance in m_Instances.
     */
    const int m_InstanceIndex;

    QString m_DatabasePath, m_TableName;
    zmc::SqliteManager m_SqlManager;
    bool m_IsTableCreated;

private:
    /*!
     * \brief Creates the table in the database. For this operation, the database must be open.
     * \return True If the operation suceeds.
     */
    bool createTable();

    /*!
     * \brief Opens the database at m_DatabasePath If it is not open. If it is open, does nothing.
     */
    void openDatabase();

    /*!
     * \brief Closes and opens the database at m_DatabasePath.
     */
    void restartDatabase();

    /*!
     * \brief Emits settingChanged() signal on all of the instances with the same \a databaseName and \a tableName.
     * \param settingName
     * \param oldSettingValue
     * \param newSettingValue
     * \param tableName
     * \param databaseName
     */
    static void emitSettingChangedInAllInstances(const QString &settingName,
        const QVariant &oldSettingValue, const QVariant &newSettingValue,
        const QString &tableName, const QString &databaseName);

    /*!
     * \brief Emits cleared() signal on all of the instances with the same \a databaseName and \a tableName.
     */
    static void emitClearedSignals();

signals:
    /*!
     * \brief This signal is emitted in all of the SettingsManager instances when a setting is changed.
     * \param settingName
     * \param oldSettingValue This will an empty string if the setting is inserted for the first time.
     * \param newSettingValue
     *
     * The signal will only be emitted for the instances with the same database name and table name as the origin
     * instance.
     */
    void settingChanged(QString settingName, QVariant oldSettingValue, QVariant newSettingValue);

    /*!
     * \brief Emitted when the database path changes.
     */
    void databaseNameChanged();

    /*!
     * \brief Emitted when the table name changes.
     */
    void tableNameChanged();

    /*!
     * \brief Emitted when the database is opened.
     */
    void databaseOpened();

    /*!
     * \brief Emitted when the database is closed.
     */
    void databaseClosed();

    /*!
     * \brief Emitted when the database is cleared.
     */
    void cleared();
};

}
