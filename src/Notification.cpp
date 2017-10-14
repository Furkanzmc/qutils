#include "qutils/Notification.h"
// Qt
#include <QDebug>

namespace zmc
{

Notification::Notification(QObject *parent)
    : QObject(parent)
    , m_PriorityFlag(static_cast<int>(Priority::Default))
    , m_Category("")
    , m_Text("")
    , m_Title("")
    , m_NotificationTag("")
    , m_Sound("")
    , m_Payload("")
    , m_TargetNotificationManagerName("")
    , m_LedColor()
    , m_LedOnMS(0)
    , m_LedOffMS(0)
    , m_DelayMS(0)
    , m_Defaults(-1)
    , m_Flags(-1)
    , m_Visibility(-2)
    , m_Date()
{
}

Notification::Notification(Notification &&other)
    : QObject(nullptr)
    , m_PriorityFlag(other.getPriority())
    , m_Category(other.getCategory())
    , m_Text(other.getText())
    , m_Title(other.getText())
    , m_NotificationTag(other.getNotificationTag())
    , m_Sound(other.getSound())
    , m_LedColor(other.getLedColor())
    , m_LedOnMS(other.getLedOnMS())
    , m_LedOffMS(other.getLedOffMS())
    , m_DelayMS(other.getDelay())
    , m_Defaults(other.getDefaults())
    , m_Flags(other.getFlags())
    , m_Visibility(other.getVisibility())
    , m_Date(other.getDate())
{

}

Notification Notification::fromVariantMap(const QVariantMap &data)
{
    Notification notification;

    if (data.find("title") == data.end() || data.find("text") == data.end()) {
        qDebug() << "The `title` or `text` keys cannot be found. Returning an empty Notification object.";
        return notification;
    }

    if (data.find("priority") != data.end()) {
        notification.setPriority(data["priority"].toInt());
    }

    if (data.find("category") != data.end()) {
        notification.setCategory(data["category"].toString());
    }

    if (data.find("ledColor") != data.end()) {
        notification.setLedColor(data["ledColor"].value<QColor>());
    }

    if (data.find("ledOnMS") != data.end()) {
        notification.setLedOnMS(data["ledOnMS"].toInt());
    }

    if (data.find("ledOffMS") != data.end()) {
        notification.setLedOffMS(data["ledOffMS"].toInt());
    }

    if (data.find("title") != data.end()) {
        notification.setTitle(data["title"].toString());
    }

    if (data.find("text") != data.end()) {
        notification.setText(data["text"].toString());
    }

    if (data.find("delay") != data.end()) {
        notification.setDelay(data["delay"].toInt());
    }

    if (data.find("date") != data.end()) {
        notification.setDate(data["date"].value<QDateTime>());
    }

    if (data.find("notificationTag") != data.end()) {
        notification.setNotificationTag(data["notificationTag"].toString());
    }

    if (data.find("sound") != data.end()) {
        notification.setSound(data["sound"].toString());
    }

    if (data.find("defaults") != data.end()) {
        notification.setDefaults(data["defaults"].toInt());
    }

    if (data.find("flags") != data.end()) {
        notification.setFlags(data["flags"].toInt());
    }

    if (data.find("notificationManagerName") != data.end()) {
        notification.setNotificationManagerName(data["notificationManagerName"].toString());
    }

    if (data.find("payload") != data.end()) {
        notification.setPayload(data["payload"].toString());
    }

    return notification;
}

bool Notification::isValid() const
{
    return getText().length() > 0 && getTitle().length() > 0;
}

void Notification::setPriority(int priority)
{
    m_PriorityFlag = priority;
    emit priorityChanged();
}

int Notification::getPriority() const
{
    return m_PriorityFlag;
}

void Notification::setCategory(QString category)
{
    m_Category = category;
    emit categoryChanged();
}

QString Notification::getCategory() const
{
    return m_Category;
}

void Notification::setLedColor(const QColor &color)
{
    m_LedColor = color;
    emit ledColorChanged();
}

QColor Notification::getLedColor() const
{
    return m_LedColor;
}

void Notification::setLedOnMS(const int &ms)
{
    m_LedOnMS = ms;
    emit ledOnMSChanged();
}

int Notification::getLedOnMS() const
{
    return m_LedOnMS;
}

void Notification::setLedOffMS(const int &ms)
{
    m_LedOffMS = ms;
    emit ledOffMSChanged();
}

int Notification::getLedOffMS() const
{
    return m_LedOffMS;
}

void Notification::setTitle(QString title)
{
    m_Title = title;
    emit titleChanged();
}

QString Notification::getTitle() const
{
    return m_Title;
}

void Notification::setText(QString text)
{
    m_Text = text;
    emit textChanged();
}

QString Notification::getText() const
{
    return m_Text;
}

void Notification::setDelay(int delay)
{
    m_DelayMS = delay;
    emit delayChanged();
}

int Notification::getDelay() const
{
    return m_DelayMS;
}

void Notification::setDate(QDateTime date)
{
    m_Date = date;
    emit dateChanged();
}

QDateTime Notification::getDate() const
{
    return m_Date;
}

void Notification::setNotificationTag(QString tag)
{
    m_NotificationTag = tag;
    emit notificationTagChanged();
}

QString Notification::getNotificationTag() const
{
    return m_NotificationTag;
}

void Notification::setSound(QString sound)
{
    m_Sound = sound;
    emit soundChanged();
}

QString Notification::getSound() const
{
    return m_Sound;
}

void Notification::setNotificationManagerName(QString name)
{
    m_TargetNotificationManagerName = name;
}

QString Notification::getNotificationManagerName() const
{
    return m_TargetNotificationManagerName;
}

void Notification::setDefaults(int defaults)
{
    m_Defaults = defaults;
    emit defaultsChanged();
}

int Notification::getDefaults() const
{
    return m_Defaults;
}

void Notification::setFlags(int flags)
{
    m_Flags = flags;
    emit flagsChanged();
}

int Notification::getFlags() const
{
    return m_Flags;
}

void Notification::setVisibility(int visibility)
{
    m_Visibility = visibility;
    emit visibilityChanged();
}

int Notification::getVisibility() const
{
    return m_Visibility;
}

//----- Notification Categories ----- //

QString Notification::getCategoryAlarm() const
{
    return "alarm";
}

QString Notification::getCategoryCall() const
{
    return "call";
}

QString Notification::getCategoryEmail() const
{
    return "email";
}

QString Notification::getCategoryError() const
{
    return "err";
}

QString Notification::getCategoryEvent() const
{
    return "event";
}

QString Notification::getCategoryMessage() const
{
    return "msg";
}

QString Notification::getCategoryProgress() const
{
    return "progress";
}

QString Notification::getCategoryPromo() const
{
    return "promo";
}

QString Notification::getCategoryRecommendation() const
{
    return "recommendation";
}

QString Notification::getCategoryReminder() const
{
    return "reminder";
}

QString Notification::getCategoryService() const
{
    return "service";
}

QString Notification::getCategorySocial() const
{
    return "social";
}

QString Notification::getCategoryStatus() const
{
    return "status";
}

QString Notification::getCategorySystem() const
{
    return "sys";
}

QString Notification::getCategoryTransport() const
{
    return "transport";
}

QString Notification::getPayload() const
{
    return m_Payload;
}

void Notification::setPayload(const QString &data)
{
    if (m_Payload != data) {
        emit dataChanged();
    }

    m_Payload = data;
}

}
