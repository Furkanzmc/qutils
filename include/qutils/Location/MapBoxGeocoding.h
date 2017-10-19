#pragma once
#include <QObject>
// qutils
#include "qutils/Location/MapBoxGeocodingQuery.h"

namespace zmc
{

// Forward Declerations
namespace Network
{
class NetworkManager;
struct Response;
}

namespace Location
{

/**
 * @brief MapBoxGeocoding uses the MapBox REST API to provide the geocoding functionality.
 */
class MapBoxGeocoding : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString token READ getToken WRITE setToken NOTIFY tokenChanged)
    Q_PROPERTY(MapBoxGeocodingQuery *query READ getQuery WRITE setQuery NOTIFY queryChanged)

public:
    explicit MapBoxGeocoding(QObject *parent = nullptr);
    ~MapBoxGeocoding();

    /**
     * @brief Returns the MapBox token for this instance.
     * @return QString
     */
    QString getToken() const;

    /**
     * @brief Sets given token to this instance's token. If the token is not the same as it was before, the tokenChanged() signal is emitted.
     * @param token
     * @return void
     */
    void setToken(const QString &token);

    /**
     * @brief Returns a pointer to the query.
     * @return MapBoxGeocodingQuery*
     */
    MapBoxGeocodingQuery *getQuery() const;

    /**
     * @brief Set the query.
     * @param query
     * @return void
     */
    void setQuery(MapBoxGeocodingQuery *query);

    /**
     * @brief Uses the given MapBoxGeocodingQuery to update the request. If there is no valid query, this will do nothing and return false.
     * @return bool
     */
    Q_INVOKABLE bool update(MapBoxGeocodingQuery *query);

signals:
    /**
     * @brief Emitted when the token changes.
     */
    void tokenChanged();

    /**
     * @brief Emitted when the query changes.
     */
    void queryChanged();

private:
    /**
     * @brief This is the base URL for MapBox's Geocoding API.
     */
    const QString m_BaseURL;

    /**
     * @brief This is the token for the MapBox API. If this token is not set, no request can be made.
     */
    QString m_Token;
    Network::NetworkManager *m_NetworkManager;
    MapBoxGeocodingQuery *m_Query;

private:
    void updateQueryCallback(const zmc::Network::Response &response);
};

}

}
