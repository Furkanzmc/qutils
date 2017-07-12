#include "qutils/DateManager.h"
// Qt
#include <QLocale>
#include <QDebug>

namespace zmc
{

DateManager::DateManager(QObject *parent)
    : QObject(parent)
    , m_Date(QDate::currentDate())
{

}

void DateManager::addMonths(const unsigned int &months)
{
    m_Date = m_Date.addMonths(months);
}

QDate DateManager::addMonths(QDate from, const unsigned int &months)
{
    return from.addMonths(months);
}

QDate DateManager::addYears(QDate from, const unsigned int &years)
{
    return from.addYears(years);
}

void DateManager::setDay(const unsigned int &day)
{
    m_Date.setDate(m_Date.year(), m_Date.month(), day);
}

void DateManager::setMonth(const unsigned int &month)
{
    m_Date.setDate(m_Date.year(), month, 1);
}

void DateManager::setYear(const unsigned int &year)
{
    m_Date.setDate(year, m_Date.month(), m_Date.day());
}

int DateManager::getYear() const
{
    return m_Date.year();
}

int DateManager::getMonth() const
{
    return m_Date.month();
}

int DateManager::getDay() const
{
    return m_Date.day();
}

void DateManager::setDate(const unsigned int &year, const unsigned int &month, const unsigned int &day)
{
    m_Date.setDate(year, month, day);
}

void DateManager::setDate(const QDate &date)
{
    m_Date = date;
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
    return getMonthLength(m_Date.year(), m_Date.month());
}

int DateManager::getMonthStartIndex() const
{
    int index = 0;
    // NOTE: Always check for en_US locale.

    QLocale locale("en_US");
    const QDate startOfTheMonth(m_Date.year(), m_Date.month(), 1);
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
    QDate date(m_Date.year(), month, 1);
    return date.toString("MMMM");
}

QString DateManager::getCurrentMonthName() const
{
    return getMonthName(m_Date.month());
}

QDate DateManager::getDate(int year, int month, int day) const
{
    return QDate(year, month, day);
}

QDate DateManager::getDate() const
{
    return m_Date;
}

QDate DateManager::getCurrentDate() const
{
    return QDate::currentDate();
}

QString DateManager::getDateString(const QString &format)
{
    return m_Date.toString(format);
}

}
