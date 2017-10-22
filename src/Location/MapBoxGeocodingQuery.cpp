#include "qutils/Location/MapBoxGeocodingQuery.h"
#include "qutils/Macros.h"

namespace zmc
{

namespace Location
{

MapBoxGeocodingQuery::MapBoxGeocodingQuery(QObject *parent)
    : QObject(parent)
    , m_Country("")
    , m_Language("")
    , m_Types("")
    , m_Proximity()
    , m_BoundingBox()
    , m_Autocomplete(true)
    , m_Limit(5)
    , m_Mode(Mode::Places)
{

}

QUrlQuery MapBoxGeocodingQuery::constructQuery() const
{
    QUrlQuery query;

    if (m_Country.length() > 0) {
        query.addQueryItem("country", m_Country);
    }

    if (m_Proximity.isNull() == false) {
        query.addQueryItem("proximity", QString("%1,%2").arg(QString::number(m_Proximity.y())).arg(QString::number(m_Proximity.x())));
    }

    if (m_Types.length() > 0) {
        query.addQueryItem("types", QString(m_Types.join(",")));
    }

    query.addQueryItem("autocomplete", m_Autocomplete ? "true" : "false");

    if (m_BoundingBox.isValid()) {
        QString format = "%1,%2,%3,%4";
        QString bbox = format.arg(m_BoundingBox.left()).arg(m_BoundingBox.top()).arg(m_BoundingBox.right()).arg(m_BoundingBox.bottom());
        query.addQueryItem("bbox", bbox);
    }

    query.addQueryItem("limit", QString::number(m_Limit));

    if (m_Language.length() > 0) {
        query.addQueryItem("language", m_Language);
    }

    return query;
}

QString MapBoxGeocodingQuery::getCountry() const
{
    return m_Country;
}

bool MapBoxGeocodingQuery::setCountry(const QString &country)
{
    m_Country = country;
    return m_Country.length();
}

QPointF MapBoxGeocodingQuery::getProximity() const
{
    return m_Proximity;
}

void MapBoxGeocodingQuery::setProximity(const QPointF &proximity)
{
    if (m_Proximity.x() != proximity.x() || m_Proximity.y() != proximity.y()) {
        m_Proximity = proximity;
        emit proximityChanged();
    }
}

QStringList MapBoxGeocodingQuery::getTypes() const
{
    return m_Types;
}

void MapBoxGeocodingQuery::setTypes(const QStringList &types)
{
    m_Types = types;
    emit typesChanged();
}

QRectF MapBoxGeocodingQuery::getBoundingBox() const
{
    return m_BoundingBox;
}

void MapBoxGeocodingQuery::setBoundingBox(const QRectF &boundingBox)
{
    if (m_BoundingBox.top() != boundingBox.top() || m_BoundingBox.left() != boundingBox.left()
        || m_BoundingBox.right() != boundingBox.right() || m_BoundingBox.bottom() != boundingBox.bottom()) {
        m_BoundingBox = boundingBox;
        emit boundingBoxChanged();
    }
}

QString MapBoxGeocodingQuery::getLanguage() const
{
    return m_Language;
}

void MapBoxGeocodingQuery::setLanguage(const QString &language)
{
    if (m_Language != language) {
        m_Language = language;
        emit languageChanged();
    }
}

float MapBoxGeocodingQuery::getLatitude() const
{
    return m_Proximity.x();
}

void MapBoxGeocodingQuery::setLatitude(const float &latitude)
{
    if (m_Proximity.x() != latitude) {
        m_Proximity.setX(latitude);
        emit latitudeChanged();
    }
}

float MapBoxGeocodingQuery::getLongitude() const
{
    return m_Proximity.y();
}

void MapBoxGeocodingQuery::setLongitude(const float &longitude)
{
    if (m_Proximity.y() != longitude) {
        m_Proximity.setY(longitude);
        emit longitudeChanged();
    }
}

QString MapBoxGeocodingQuery::getSearchString() const
{
    return m_SearchString;
}

void MapBoxGeocodingQuery::setSearchString(const QString &query)
{
    if (m_SearchString != query) {
        m_SearchString = query;
        emit searchQueryChanged();
    }
}

MapBoxGeocodingQuery::Mode MapBoxGeocodingQuery::getMode() const
{
    return m_Mode;
}

void MapBoxGeocodingQuery::setMode(const MapBoxGeocodingQuery::Mode &mode)
{
    if (m_Mode != mode) {
        m_Mode = mode;
        emit modeChanged();
    }
}

QString MapBoxGeocodingQuery::getModeString() const
{
    return m_Mode == Mode::Places ? "mapbox.places" : "mapbox.places-permanent";
}

}

}
