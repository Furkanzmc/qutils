#pragma once
// Qt
#include <QObject>
// Local
#include "qutils/SqliteManager.h"
#include "qutils/Macros.h"

#ifdef QUTILS_APP_NAME
    #define CACHE_DB_FILE_NAME STRINGIFY(QUTILS_APP_NAME) "_cache.sqlite"
#else
    #define CACHE_DB_FILE_NAME "qutils_cache.sqlite"
#endif // QUTILS_APP_NAME

namespace zmc
{

/*!
 * \brief The CacheManager class uses a SqliteManager to store the settings. Settings are saved in QVariant format.
 * When a setting is changed the settingChanged signal is emitted and this signal is emitted in all of the
 */
class CacheManager : public QObject
{
    Q_OBJECT Q_PROPERTY(QString databasePath READ getDatabaseName WRITE setDatabaseName NOTIFY databaseNameChanged)
    Q_PROPERTY(QString cacheTableName READ getTableName WRITE setTableName NOTIFY tableNameChanged)

public:
    explicit CacheManager(const QString &databaseName = CACHE_DB_FILE_NAME, const QString &tableName = "cache", QObject *parent = 0);
    ~CacheManager();

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
     * \brief Clears the cache.
     * \return bool Returns true If the operation succeeds.
     */
    Q_INVOKABLE bool clear();

    /*!
     * \brief Returns the database name in use.
     * \return
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
     * \brief Sets the table name to the given table.
     * \param tableName
     */
    void setTableName(const QString &tableName);

    /*!
     * \brief Returns the value of QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
     * \return QString
     */
    QString getWritableLocation() const;

private:
    /*!
     * \variable static QMap<int, CacheManager *> m_Instances
     * \brief This variable is used to keep track of all the instances so that we can send signals to all of them.
     */
    static QMap<int, CacheManager *> m_Instances;

    /*!
     * \variable const int m_InstanceIndex
     * \brief This variable is initilized in the constructor to the size value of m_Instances.
     *
     * This is used to identify the current instance in m_Instances.
     */
    const int m_InstanceIndex;

    QString m_DatabaseName, m_TableName;
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
     * \brief Emits cacheChanged signal in all of the instances that has the same \a tableName and \a databaseName.
     * \param cacheName
     * \param oldCachedValue
     * \param newCachedValue
     * \param databaseName
     * \param tableName
     */
    void emitCacheChangedInAllInstances(const QString &cacheName, const QVariant &oldCachedValue, const QVariant &newCachedValue, const QString &databaseName,
        const QString &tableName);

signals:
    /*!
     * \brief This is emitted when a cache is changed. This signal is emitted in all of the CacheManager instances when a cache is changed.
     * \param cacheName
     * \param oldCachedValue This will an empty string if the setting is inserted for the first time.
     * \param newCachedValue
     */
    void cacheChanged(QString cacheName, QVariant oldCachedValue, QVariant newCachedValue);

    /*!
     * \brief Emitted when database name changes.
     */
    void databaseNameChanged();

    /*!
     * \brief Emitted when table name changes.
     */
    void tableNameChanged();

    /*!
     * \brief Emitted when the database is opened. This is also emitted when the database name is changed and the new
     *        database is opened.
     */
    void databaseOpened();

    /*!
     * \brief Emitted when the database is closed.
     */
    void databaseClosed();

    /*!
     * \brief Emitted when the table contents are cleared. This is not emitted If the clear() method was not successful.
     */
    void cleared();
};

}
