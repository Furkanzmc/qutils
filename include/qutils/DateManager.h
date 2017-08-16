#pragma once
#include <QObject>
#include <QDateTime>

namespace zmc
{

/**
 * @brief The DateManager class
 * DateManager can perform actions on given dates or date times and it also contains an internal QDateTime object that you can manipulate.
 */
class DateManager : public QObject
{
    Q_OBJECT
public:
    explicit DateManager(QObject *parent = 0);

public:
    /**
     * @brief Adds the given months to the current date that this instance holds and returns the resulting date.
     * @param months
     * @return QDate
     */
    Q_INVOKABLE QDate addMonths(const unsigned int &months);

    /**
     * @brief Add days to the given date and returns the new instance.
     * @param from
     * @param months
     * @return QDate
     */
    Q_INVOKABLE QDate addDays(QDate from, const unsigned int &days);

    /**
     * @brief Add months to the given date and returns the new instance
     * @param from
     * @param months
     * @return QDate
     */
    Q_INVOKABLE QDate addMonths(QDate from, const unsigned int &months);

    /**
     * @brief Add years to the given date and returns the new instance
     * @param from
     * @param years
     * @return QDate
     */
    Q_INVOKABLE QDate addYears(QDate from, const unsigned int &years);

    /**
     * @brief Sets the day but keeps the month and the year the same. This will affect the date that this instance holds.
     * @param day
     */
    Q_INVOKABLE void setDay(const unsigned int &day);

    /**
     * @brief Only sets the month and resets the day to 1. This will affect the date that this instance holds.
     * @param month
     */
    Q_INVOKABLE void setMonth(const unsigned int &month);

    /**
     * @brief Sets the year but keeps the month and the day the same. This will affect the date that this instance holds.
     * @param year
     */
    Q_INVOKABLE void setYear(const unsigned int &year);

    /**
     * @brief Sets the hour but keeps the rest the same. This will affect the date that this instance holds.
     * @param hour
     */
    Q_INVOKABLE void setHour(const unsigned int &hour);

    /**
     * @brief Sets the minute but keeps the rest the same. This will affect the date that this instance holds.
     * @param minute
     */
    Q_INVOKABLE void setMinute(const unsigned int &minute);

    /**
     * @brief Sets the minute and the hour but keeps the rest the same. This will affect the date that this instance holds.
     * @param hour
     * @param minute
     */
    Q_INVOKABLE void setTime(const unsigned int &hour, const unsigned int &minute);

    /**
     * @brief Returns the year that this instance is holding.
     * @return int
     */
    Q_INVOKABLE int getYear() const;

    /**
     * @brief Returns the day that this instance is holding.
     * @return int
     */
    Q_INVOKABLE int getDay() const;

    /**
     * @brief Returns the month that this instance is holding.
     * @return int
     */
    Q_INVOKABLE int getMonth() const;

    /**
     * @brief Returns the hour that this instance is holding.
     * @return int
     */
    Q_INVOKABLE int getHour() const;

    /**
     * @brief Returns the minute that this instance is holding.
     * @return int
     */
    Q_INVOKABLE int getMinute() const;

    /**
     * @brief Changes the currently held date to the given year, month and day.
     * @param year
     * @param month
     * @param day
     */
    Q_INVOKABLE void setDate(const unsigned int &year, const unsigned int &month, const unsigned int &day);

    /**
     * @brief Changes the currently held date to the given date.
     * @param date
     */
    Q_INVOKABLE void setDate(const QDateTime &date);

    /**
     * @brief Returns the number of days in the given month in the given year.
     * @param year
     * @param month
     * @return int
     */
    Q_INVOKABLE int getMonthLength(unsigned int year, unsigned int month) const;

    /**
     * @brief Returns the number of days in the currently held date in this instance.
     * @param year
     * @param month
     * @return int
     */
    Q_INVOKABLE int getCurrentMonthLength() const;

    Q_INVOKABLE int getMonthStartIndex() const;

    /**
     * @brief Returns the name of the given month in the current locale.
     * @param month
     * @return QString
     */
    Q_INVOKABLE QString getMonthName(const unsigned int &month) const;

    /**
     * @brief Returns the name of the month in the current locale.
     * @return QString
     */
    Q_INVOKABLE QString getCurrentMonthName() const;

    /**
     * @brief Creates a new instance from given parameters.
     * @param year
     * @param month
     * @param day
     * @return QDate
     */
    Q_INVOKABLE QDate getDate(int year, int month, int day) const;

    /**
     * @brief Returns the date this instance is set to.
     * @return QDate
     */
    Q_INVOKABLE QDate getDate() const;

    /**
     * @brief Returns the date time this instance is set to.
     * @return QDateTime
     */
    Q_INVOKABLE QDateTime getDateTime() const;

    /**
     * @brief Returns the current date.
     * @return QDate
     */
    Q_INVOKABLE QDate getCurrentDate() const;

    /**
     * @brief Returns the current date time.
     * @return QDateTime
     */
    Q_INVOKABLE QDateTime getCurrentDateTime() const;

    /**
     * @brief Returns the date string in the given format. The `format` is mandatory but if the `date` parameter is skipped, the internal date will be used.
     * @param format
     * @param date
     * @return QString
     */
    Q_INVOKABLE QString getDateString(const QString &format, QDate date = QDate()) const;

    /**
     * @brief Returns a QDate instance from the given date string and date format.
     * @param dateStr
     * @param dateFormat
     * @return QDate
     */
    Q_INVOKABLE QDate fromDateString(const QString &dateStr, const QString &dateFormat) const;

    /**
     * @brief Returns a QDate instance from the given date string and date format.
     * @param dateStr
     * @param dateFormat You can use Qt::DateFormat
     * @return QDate
     */
    Q_INVOKABLE QDate fromDateString(const QString &dateStr, const Qt::DateFormat &dateFormat) const;

    /**
     * @brief Returns a QDateTime instance from the given date string and date format.
     * @param dateTimeStr
     * @param dateTimeFormat
     * @param isUTC Set to true if the date represents a UTC time.
     * @return QDateTime
     */
    Q_INVOKABLE QDateTime fromDateTimeString(const QString &dateTimeStr, const QString &dateTimeFormat, bool isUTC = false) const;

    /**
     * @brief Returns a QDateTime instance from the given date string and date format.
     * @param dateTimeStr
     * @param dateTimeFormat You can use Qt::DateFormat
     * @param isUTC Set to true if the date represents a UTC time.
     * @return QDateTime
     */
    Q_INVOKABLE QDateTime fromDateTimeString(const QString &dateTimeStr, const Qt::DateFormat &dateTimeFormat, bool isUTC = false) const;

    /**
     * @brief Converts the given QDateTime to local time.
     * @param datetime
     * @return QDateTime
     */
    Q_INVOKABLE QDateTime convertToLocalDateTime(QDateTime datetime);

    /**
     * @brief Returns the day difference between the two dates.
     * @param from
     * @param to
     * @return qint64
     */
    Q_INVOKABLE qint64 dayDifference(QDateTime from, QDateTime to);

    /**
     * @brief Returns the second difference between the two dates.
     * @param from
     * @param to
     * @return qint64
     */
    Q_INVOKABLE qint64 secondDifference(QDateTime from, QDateTime to);

    /**
     * @brief Returns the minute difference between the two dates.
     * @param from
     * @param to
     * @return float
     */
    Q_INVOKABLE float minuteDifference(QDateTime from, QDateTime to);

    /**
     * @brief Returns the hour difference between the two dates.
     * @param from
     * @param to
     * @return float
     */
    Q_INVOKABLE float hourDifference(QDateTime from, QDateTime to);

    /**
     * @brief Returns the if `from` is earlier than `to`.
     * @param from
     * @param to
     * @return bool
     */
    Q_INVOKABLE bool isEarlier(const QDateTime &from, const QDateTime &to);

    /**
     * @brief Returns the if `from` is later than `to`.
     * @param from
     * @param to
     * @return bool
     */
    Q_INVOKABLE bool isLater(const QDateTime &from, const QDateTime &to);

private:
    QDateTime m_DateTime;
};

}
