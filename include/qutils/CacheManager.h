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

/**
 * @brief The CacheManager class uses a SqliteManager to store the settings. Settings are saved in QVariant format.
 * When a setting is changed the settingChanged signal is emitted and this signal is emitted in all of the
 */
class CacheManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString databasePath READ getDatabaseName WRITE setDatabaseName NOTIFY databaseNameChanged)
    Q_PROPERTY(QString cacheTableName READ getCacheTableName WRITE setCacheTableName NOTIFY cacheTableNameChanged)

public:
    explicit CacheManager(QString databaseName = CACHE_DB_FILE_NAME, QString tableName = "cache", QObject *parent = 0);
    ~CacheManager();

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
     * @brief Clears the cache.
     * @return bool Returns true If the operation succeeds.
     */
    Q_INVOKABLE bool clear();

    QString getDatabaseName() const;

    /**
     * @brief The database will be reloaded when this property changes. This is the database file name only. Cache is alwasy created at the writable location.
     * @param databasePath
     */
    void setDatabaseName(const QString &databaseName);

    QString getCacheTableName() const;
    void setCacheTableName(const QString &tableName);
    QString getWritableLocation() const;

private:
    static QList<CacheManager *> m_Instances;
    static bool m_IsTableCreated;

    const int m_InstanceIndex;
    QString m_DatabaseName, m_CacheTableName;

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

    void emitCacheChangedInAllInstances(const QString &cacheName, const QVariant &oldCachedValue, const QVariant &newCachedValue);

signals:
    /**
     * @brief This is emitted when a cache is changed. This signal is emitted in all of the CacheManager instances when a cache is changed.
     * @param cacheName
     * @param oldCachedValue This will an empty string if the setting is inserted for the first time.
     * @param newCachedValue
     */
    void cacheChanged(QString cacheName, QVariant oldCachedValue, QVariant newCachedValue);
    void databaseNameChanged();
    void cacheTableNameChanged();

    void databaseOpened();
    void databaseClosed();
    void cleared();
};

}
