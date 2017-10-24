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
    if (qFuzzyCompare(m_Proximity.x() + 1.0, proximity.x() + 1.0) || qFuzzyCompare(m_Proximity.y() + 1.0, proximity.y() + 1.0)) {
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
    if (qFuzzyCompare(m_BoundingBox.top() + 1.0, boundingBox.top() + 1.0) || qFuzzyCompare(m_BoundingBox.left() + 1.0, boundingBox.left() + 1.0)
        || qFuzzyCompare(m_BoundingBox.right() + 1.0, boundingBox.right() + 1.0) || qFuzzyCompare(m_BoundingBox.bottom() + 1.0, boundingBox.bottom() + 1.0)) {
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

qreal MapBoxGeocodingQuery::getLatitude() const
{
    return m_Proximity.x();
}

void MapBoxGeocodingQuery::setLatitude(const qreal &latitude)
{
    if (!qFuzzyCompare(m_Proximity.x() + 1.0, latitude + 1.0)) {
        m_Proximity.setX(latitude);
        emit latitudeChanged();
    }
}

qreal MapBoxGeocodingQuery::getLongitude() const
{
    return m_Proximity.y();
}

void MapBoxGeocodingQuery::setLongitude(const qreal &longitude)
{
    if (!qFuzzyCompare(m_Proximity.y() + 1.0, longitude + 1.0)) {
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

int MapBoxGeocodingQuery::getLimit() const
{
    return m_Limit;
}

void MapBoxGeocodingQuery::setLimit(int Limit)
{
    m_Limit = Limit;
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
