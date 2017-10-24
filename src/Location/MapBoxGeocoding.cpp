#include "qutils/Location/MapBoxGeocoding.h"
#include "qutils/Network/NetworkManager.h"
#include "qutils/Macros.h"
#include "qutils/Network/HttpCodes.h"
#include "qutils/JsonUtils.h"
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
    , m_AutoUpdate(false)
{
    connect(this, &MapBoxGeocoding::queryChanged, this, &MapBoxGeocoding::updateQueryConnection);
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

bool MapBoxGeocoding::update()
{
    if (m_Query == nullptr) {
        LOG_ERROR("There is no valid query. Doing nothing.");
        return false;
    }

    if (m_Token.length() == 0) {
        LOG_ERROR("Token is not set.");
        return false;
    }

    QString urlStr = m_BaseURL;
    if (m_Query->getSearchString().length() > 0) {
        urlStr += "/geocoding/v5/" + m_Query->getModeString() + "/" + m_Query->getSearchString() + ".json";

        QUrl url(urlStr);
        QUrlQuery queryParams = m_Query->constructQuery();
        queryParams.addQueryItem("access_token", m_Token);
        url.setQuery(queryParams);
        m_NetworkManager->sendGet(url.toEncoded(), std::bind(&MapBoxGeocoding::updateQueryCallback, this, std::placeholders::_1));
        return true;
    }

    return false;
}

bool MapBoxGeocoding::getAutoUpdate() const
{
    return m_AutoUpdate;
}

void MapBoxGeocoding::setAutoUpdate(bool enabled)
{
    if (m_AutoUpdate != enabled) {
        m_AutoUpdate = enabled;
        if (m_AutoUpdate && m_Query != nullptr) {
            connect(m_Query, &MapBoxGeocodingQuery::searchQueryChanged, this, &MapBoxGeocoding::update);
        }
        else if (m_AutoUpdate && m_Query) {
            disconnect(m_Query, &MapBoxGeocodingQuery::searchQueryChanged, this, &MapBoxGeocoding::update);
        }

        emit autoUpdateChanged();
    }
}

void MapBoxGeocoding::updateQueryCallback(const Response &response)
{
    QVariantMap data = JsonUtils::toVariantMap(response.data);
    if (response.httpCode == HttpCodes::HTTP_200_OK) {
        emit responseRetreived(data);
    }
    else {
        data["http_code"] = response.httpCode;
        data["network_error_code"] = response.networkError;
        emit errorOccurred(data);
    }
}

void MapBoxGeocoding::updateQueryConnection()
{
    if (m_AutoUpdate && m_Query) {
        connect(m_Query, &MapBoxGeocodingQuery::searchQueryChanged, this, &MapBoxGeocoding::update);
    }
}

}

}
