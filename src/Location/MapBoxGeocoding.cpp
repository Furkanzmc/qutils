#include "qutils/Location/MapBoxGeocoding.h"
#include "qutils/Network/NetworkManager.h"
#include "qutils/Location/MapBoxGeocodingQuery.h"

namespace zmc
{

using namespace Network;

namespace Location
{

MapBoxGeocoding::MapBoxGeocoding(QObject *parent)
    : QObject(parent)
    , m_BaseURL("https://api.mapbox.com/")
    , m_Token("")
    , m_NetworkManager(new NetworkManager(this))
{

}

MapBoxGeocoding::~MapBoxGeocoding()
{

}

QString MapBoxGeocoding::getToken() const
{
    return m_Token;
}

void MapBoxGeocoding::setToken(const QString &token)
{
    if (m_Token != token) {
        m_Token = token;
        emit tokenChanged();
    }
}

MapBoxGeocodingQuery *MapBoxGeocoding::getQuery() const
{
    return m_Query;
}

void MapBoxGeocoding::setQuery(MapBoxGeocodingQuery *query)
{
    if (m_Query != query) {
        m_Query = query;
        emit queryChanged();
    }
}

}

}
