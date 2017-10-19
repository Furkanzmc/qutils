#pragma once
#include <QObject>
#include <QPoint>
#include <QRectF>
#include <QUrlQuery>

namespace zmc
{

namespace Location
{

/**
 * @brief MapBoxGeocodingQuery is used to ease the query parameter setting for a geocoding request.
 * For more information about the parameters see here: https://www.mapbox.com/api-documentation/?language=cURL#request-format
 */
class MapBoxGeocodingQuery : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString country READ getCountry WRITE setCountry NOTIFY countryChanged)
    Q_PROPERTY(QPointF proximity READ getProximity WRITE setProximity NOTIFY proximityChanged)
    Q_PROPERTY(QStringList types READ getTypes WRITE setTypes NOTIFY typesChanged)

    Q_PROPERTY(QRectF boundingBox READ getBoundingBox WRITE setBoundingBox NOTIFY boundingBoxChanged)
    Q_PROPERTY(QString language READ getLanguage WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(float latitude READ getLatitude WRITE setLatitude NOTIFY latitudeChanged)

    Q_PROPERTY(float longitude READ getLongitude WRITE setLongitude NOTIFY longitudeChanged)

public:
    explicit MapBoxGeocodingQuery(QObject *parent = nullptr);

    /**
     * @brief Returns the query object.
     * @return QUrlQuery
     */
    QUrlQuery constructQuery() const;

    /**
     * @brief Returns the country.
     * @return QString
     */
    QString getCountry() const;

    /**
     * @brief Sets the country for the request. Since the API only accepts ISO 3166 alpha 2, this function will return false If the character
     * count is more than 2. But it will still set the given country.
     * @param country
     * @return bool
     */
    bool setCountry(const QString &country);

    /**
     * @brief Returns the proximity in QPoint. x property of QPoint is used for latitude and the y property is used for longitude.
     * @return QPointF
     */
    QPointF getProximity() const;

    /**
     * @brief x property of QPoint is used for latitude and the y property is used for longitude.
     * @param proximity
     * @return void
     */
    void setProximity(const QPointF &proximity);

    /**
     * @brief Returns the list of types set for this instance.
     * @return QStringList
     */
    QStringList getTypes() const;

    /**
     * @brief Set the types for this query. See here for options: https://www.mapbox.com/api-documentation/?language=cURL#request-format
     * @param types
     */
    void setTypes(const QStringList &types);

    /**
     * @brief (left, top) is used for (minX, minY) and (right, bottom) is used for (maxX, maxY).
     * @return QRectF
     */
    QRectF getBoundingBox() const;

    /**
     * @brief (left, top) is used for (minX, minY) and (right, bottom) is used for (maxX, maxY).
     * @param boundingBox
     * @return void
     */
    void setBoundingBox(const QRectF &boundingBox);

    /**
     * @brief Returns the langauge code for this query.
     * @return QString
     */
    QString getLanguage() const;

    /**
     * @brief Sets the langauge code for this query.
     * @param language
     * @return void
     */
    void setLanguage(const QString &language);

    /**
     * @brief Returns the latitude of the proximity.
     * @return float
     */
    float getLatitude() const;

    /**
     * @brief Sets the latitude of the proximity.
     * @param latitude
     * @return void
     */
    void setLatitude(const float &latitude);

    /**
     * @brief Returns the longitude of the proximity.
     * @return float
     */
    float getLongitude() const;

    /**
     * @brief Sets the longitude of the proximity.
     * @param latitude
     * @return void
     */
    void setLongitude(const float &longitude);

signals:
    /**
     * @brief Emitted when the country changes.
     */
    void countryChanged();

    /**
     * @brief Emitted when the proximity changes.
     */
    void proximityChanged();

    /**
     * @brief Emitted when the types changes.
     */
    void typesChanged();

    /**
     * @brief Emitted when the bounding box changes.
     */
    void boundingBoxChanged();

    /**
     * @brief Emitted when the language changes.
     */
    void languageChanged();

    /**
     * @brief Emitted when proximity latitude changes.
     */
    void latitudeChanged();

    /**
     * @brief Emitted when proximity latitude changes.
     */
    void longitudeChanged();

private:
    QString m_Country, m_Language;

    QStringList m_Types;
    QPointF m_Proximity;
    QRectF m_BoundingBox;

    bool m_Autocomplete;
    int m_Limit;

public slots:
};

}

}
