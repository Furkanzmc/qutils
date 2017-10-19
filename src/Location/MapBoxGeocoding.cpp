#include "qutils/Location/MapBoxGeocoding.h"
#include "qutils/Network/NetworkManager.h"
#include "qutils/Macros.h"
// Qt
#include <QUrlQuery>

namespace zmc
{

using namespace Network;

namespace Location
{

MapBoxGeocoding::MapBoxGeocoding(QObject *parent)
    : QObject(parent)
    , m_BaseURL("https://api.mapbox.com")
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

bool MapBoxGeocoding::update(MapBoxGeocodingQuery *query)
{
    MapBoxGeocodingQuery *queryToUse = query ? query : m_Query;
    if (queryToUse == nullptr) {
        LOG_ERROR("There is no valid query. Doing nothing.");
        return false;
    }

    if (m_Token.length() == 0) {
        LOG_ERROR("Token is not set.");
        return false;
    }

    QString urlStr = m_BaseURL;
    if (queryToUse->getSearchString().length() > 0) {
        urlStr += "/geocoding/v5/" + queryToUse->getModeString() + "/" + queryToUse->getSearchString() + ".json";
    }

    QUrl url(urlStr);
    QUrlQuery queryParams = queryToUse->constructQuery();
    queryParams.addQueryItem("access_token", m_Token);
    url.setQuery(queryParams);
    m_NetworkManager->sendGet(url.toEncoded(), std::bind(&MapBoxGeocoding::updateQueryCallback, this, std::placeholders::_1));
    return true;
}

void MapBoxGeocoding::updateQueryCallback(const Response &response)
{
    LOG(response.data);
}

}

}
