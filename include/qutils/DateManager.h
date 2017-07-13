#pragma once
#include <QObject>
#include <QDateTime>

namespace zmc
{

class DateManager : public QObject
{
    Q_OBJECT
public:
    explicit DateManager(QObject *parent = 0);

public:
    /**
     * @brief Adds the given months to the current date
     * @param months
     */
    Q_INVOKABLE QDate addMonths(const unsigned int &months);

    /**
     * @brief Add days to the given date and returns the new instance
     * @param from
     * @param months
     * @return
     */
    Q_INVOKABLE QDate addDays(QDate from, const unsigned int &days);

    /**
     * @brief Add months to the given date and returns the new instance
     * @param from
     * @param months
     * @return
     */
    Q_INVOKABLE QDate addMonths(QDate from, const unsigned int &months);

    /**
     * @brief Add years to the given date and returns the new instance
     * @param from
     * @param years
     * @return
     */
    Q_INVOKABLE QDate addYears(QDate from, const unsigned int &years);

    /**
     * @brief Sets the day but keeps the month and the year the same
     * @param day
     */
    Q_INVOKABLE void setDay(const unsigned int &day);

    /**
     * @brief Only sets the month and resets the day to 1
     * @param month
     */
    Q_INVOKABLE void setMonth(const unsigned int &month);

    /**
     * @brief Sets the year but keeps the month and the day the same.
     * @param year
     */
    Q_INVOKABLE void setYear(const unsigned int &year);

    Q_INVOKABLE void setHour(const unsigned int &hour);
    Q_INVOKABLE void setMinute(const unsigned int &minute);
    Q_INVOKABLE void setTime(const unsigned int &hour, const unsigned int &minute);

    Q_INVOKABLE int getYear() const;
    Q_INVOKABLE int getDay() const;
    Q_INVOKABLE int getMonth() const;

    Q_INVOKABLE int getHour() const;
    Q_INVOKABLE int getMinute() const;

    Q_INVOKABLE void setDate(const unsigned int &year, const unsigned int &month, const unsigned int &day);
    Q_INVOKABLE void setDate(const QDateTime &date);

    Q_INVOKABLE int getMonthLength(unsigned int year, unsigned int month) const;
    Q_INVOKABLE int getCurrentMonthLength() const;

    Q_INVOKABLE int getMonthStartIndex() const;
    Q_INVOKABLE QString getMonthName(const unsigned int &month) const;
    Q_INVOKABLE QString getCurrentMonthName() const;

    /**
     * @brief Creates a new instance from given parameters.
     * @param year
     * @param month
     * @param day
     * @return
     */
    Q_INVOKABLE QDate getDate(int year, int month, int day) const;

    /**
     * @brief Returns the date this instance is set to
     * @return
     */
    Q_INVOKABLE QDate getDate() const;

    /**
     * @brief Returns the date time this instance is set to
     * @return
     */
    Q_INVOKABLE QDateTime getDateTime() const;

    /**
     * @brief Returns the current date
     * @return
     */
    Q_INVOKABLE QDate getCurrentDate() const;
    Q_INVOKABLE QDateTime getCurrentDateTime() const;

    Q_INVOKABLE QString getDateString(const QString &format, QDate date = QDate());
    Q_INVOKABLE QDate fromDateString(const QString &dateStr, const QString &dateFormat) const;
    Q_INVOKABLE QDate fromDateString(const QString &dateStr, const Qt::DateFormat &dateFormat) const;

    Q_INVOKABLE QDateTime fromDateTimeString(const QString &dateTimeStr, const QString &dateTimeFormat, bool isUTC = false) const;
    Q_INVOKABLE QDateTime fromDateTimeString(const QString &dateTimeStr, const Qt::DateFormat &dateTimeFormat, bool isUTC = false) const;
    Q_INVOKABLE QDateTime convertToLocalDateTime(QDateTime datetime);

private:
    QDateTime m_DateTime;
};

}
