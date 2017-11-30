#include "qutils/DateManager.h"
// Qt
#include <QLocale>
#include <QDebug>
#include <QTimeZone>

namespace zmc
{

DateManager::DateManager(QObject *parent)
    : QObject(parent)
    , m_DateTime(QDateTime::currentDateTime())
{

}

QDate DateManager::addMonths(const unsigned int &months)
{
    m_DateTime = m_DateTime.addMonths(months);
    return m_DateTime.date();
}

QDate DateManager::addDays(QDate from, const unsigned int &days)
{
    return from.addDays(days);
}

QDate DateManager::addMonths(QDate from, const unsigned int &months)
{
    return from.addMonths(months);
}

QDate DateManager::addYears(QDate from, const unsigned int &years)
{
    return from.addYears(years);
}

QDateTime DateManager::addSeconds(QDateTime dateTime, const int &seconds) const
{
    return dateTime.addSecs(seconds);
}

void DateManager::setDay(const unsigned int &day)
{
    const QDate date = m_DateTime.date();
    m_DateTime.setDate(QDate(date.year(), date.month(), day));
}

void DateManager::setMonth(const unsigned int &month)
{
    const QDate date = m_DateTime.date();
    m_DateTime.setDate(QDate(date.year(), month, 1));
}

void DateManager::setYear(const unsigned int &year)
{
    const QDate date = m_DateTime.date();
    m_DateTime.setDate(QDate(year, date.month(), date.day()));
}

void DateManager::setHour(const unsigned int &hour)
{
    const QTime time(hour, m_DateTime.time().minute(), 0);
    m_DateTime.setTime(time);
}

void DateManager::setMinute(const unsigned int &minute)
{
    const QTime time(m_DateTime.time().hour(), minute, 0);
    m_DateTime.setTime(time);
}

void DateManager::setTime(const unsigned int &hour, const unsigned int &minute)
{
    const QTime time(hour, minute, 0);
    m_DateTime.setTime(time);
}

int DateManager::getYear() const
{
    return m_DateTime.date().year();
}

int DateManager::getMonth() const
{
    return m_DateTime.date().month();
}

int DateManager::getHour() const
{
    return m_DateTime.time().hour();
}

int DateManager::getMinute() const
{
    return m_DateTime.time().minute();
}

int DateManager::getDay() const
{
    return m_DateTime.date().day();
}

void DateManager::setDate(const unsigned int &year, const unsigned int &month, const unsigned int &day)
{
    m_DateTime.setDate(QDate(year, month, day));
}

void DateManager::setDate(const QDateTime &date)
{
    m_DateTime = date;
}

int DateManager::getMonthLength(unsigned int year, unsigned int month) const
{
    if (month > 12) {
        month = 1;
    }
    else if (month < 1) {
        month = 12;
    }

    QDate then(year, month, 1);
    return then.daysInMonth();
}

int DateManager::getCurrentMonthLength() const
{
    return getMonthLength(m_DateTime.date().year(), m_DateTime.date().month());
}

int DateManager::getMonthStartIndex() const
{
    int index = 0;
    // NOTE: Always check for en_US locale.
    QLocale locale("en_US");
    const QDate date = m_DateTime.date();
    const QDate startOfTheMonth(date.year(), date.month(), 1);
    const QString shortDayName = locale.toString(startOfTheMonth, "ddd");

    if (shortDayName == "Tue") {
        index = 1;
    }
    else if (shortDayName == "Wed") {
        index = 2;
    }
    else if (shortDayName == "Thu") {
        index = 3;
    }
    else if (shortDayName == "Fri") {
        index = 4;
    }
    else if (shortDayName == "Sat") {
        index = 5;
    }
    else if (shortDayName == "Sun") {
        index = 6;
    }

    return index;
}

QString DateManager::getMonthName(const unsigned int &month) const
{
    QDate date(m_DateTime.date().year(), month, 1);
    return date.toString("MMMM");
}

QString DateManager::getCurrentMonthName() const
{
    return getMonthName(m_DateTime.date().month());
}

QDate DateManager::getDate(int year, int month, int day) const
{
    return QDate(year, month, day);
}

QDate DateManager::getDate() const
{
    return m_DateTime.date();
}

QDateTime DateManager::getDateTime() const
{
    return m_DateTime;
}

QDate DateManager::getCurrentDate() const
{
    return QDate::currentDate();
}

QDateTime DateManager::getCurrentDateTime() const
{
    return QDateTime::currentDateTime();
}

QString DateManager::getDateString(const QString &format, QDate date) const
{
    if (date.isValid()) {
        return date.toString(format);
    }

    return m_DateTime.toString(format);
}

QDate DateManager::fromDateString(const QString &dateStr, const QString &dateFormat) const
{
    return QDate::fromString(dateStr, dateFormat);
}

QDate DateManager::fromDateString(const QString &dateStr, const Qt::DateFormat &dateFormat) const
{
    return QDate::fromString(dateStr, dateFormat);
}

QDateTime DateManager::fromDateTimeString(const QString &dateTimeStr, const QString &dateTimeFormat, bool isUTC) const
{
    QDateTime datetime = QDateTime::fromString(dateTimeStr, dateTimeFormat);
    if (isUTC) {
        datetime.setTimeZone(QTimeZone::utc());
    }
    else {
        datetime.setTimeZone(QTimeZone::systemTimeZone());
    }

    return datetime;
}

QDateTime DateManager::fromDateTimeString(const QString &dateTimeStr, const Qt::DateFormat &dateTimeFormat, bool isUTC) const
{
    QDateTime datetime = QDateTime::fromString(dateTimeStr, dateTimeFormat);
    if (isUTC) {
        datetime.setTimeZone(QTimeZone::utc());
    }
    else {
        datetime.setTimeZone(QTimeZone::systemTimeZone());
    }

    return datetime;
}

QDateTime DateManager::convertToLocalDateTime(QDateTime datetime)
{
    return datetime.toLocalTime();
}

QDateTime DateManager::convertToUTCDateTime(QDateTime datetime)
{
    return datetime.toUTC();
}

qint64 DateManager::dayDifference(QDateTime from, QDateTime to)
{
    return from.daysTo(to);
}

qint64 DateManager::secondDifference(QDateTime from, QDateTime to)
{
    return from.secsTo(to);
}

float DateManager::minuteDifference(QDateTime from, QDateTime to)
{
    return from.secsTo(to) / 60.f;
}

float DateManager::hourDifference(QDateTime from, QDateTime to)
{
    return minuteDifference(from, to) / 60.f;
}

bool DateManager::isEarlier(const QDateTime &from, const QDateTime &to)
{
    return from < to;
}

bool DateManager::isLater(const QDateTime &from, const QDateTime &to)
{
    return from > to;
}

int DateManager::getOffsetFromUtc() const
{
    const QDateTime now = QDateTime::currentDateTime();
    return now.offsetFromUtc();
}

}
