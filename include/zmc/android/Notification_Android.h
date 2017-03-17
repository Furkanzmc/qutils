#pragma once
#include <QObject>
#include <QColor>
#include <QDateTime>
#include <QVariantMap>

namespace zmc
{

class Notification : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int priority READ getPriority WRITE setPriority NOTIFY priorityChanged)
    Q_PROPERTY(QString category READ getCategory WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QColor ledColor READ getLedColor WRITE setLedColor NOTIFY ledColorChanged)

    Q_PROPERTY(int ledOnMS READ getLedOnMS WRITE setLedOnMS NOTIFY ledOnMSChanged)
    Q_PROPERTY(int ledOffMS READ getLedOffMS WRITE setLedOffMS NOTIFY ledOffMSChanged)
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)

    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int delay READ getDelay WRITE setDelay NOTIFY delayChanged)
    Q_PROPERTY(QDateTime date READ getDate WRITE setDate NOTIFY dateChanged)

    Q_PROPERTY(QString notificationTag READ getNotificationTag WRITE setNotificationTag NOTIFY notificationTagChanged)
    Q_PROPERTY(int notificationID READ getNotificationID WRITE setNotificationID NOTIFY notificationIDChanged)
    Q_PROPERTY(QString sound READ getSound WRITE setSound NOTIFY soundChanged)

    Q_PROPERTY(int defaults READ getDefaults WRITE setDefaults NOTIFY defaultsChanged)
    Q_PROPERTY(int flags READ getFlags WRITE setFlags NOTIFY flagsChanged)
    Q_PROPERTY(int visibility READ getVisibility WRITE setVisibility NOTIFY visibilityChanged)


    //----- Notification Categories ----- //

    Q_PROPERTY(QString CATEGORY_ALARM READ getCategoryAlarm)
    Q_PROPERTY(QString CATEGORY_CALL READ getCategoryCall)
    Q_PROPERTY(QString CATEGORY_EMAIL READ getCategoryEmail)

    Q_PROPERTY(QString CATEGORY_ERROR READ getCategoryError)
    Q_PROPERTY(QString CATEGORY_EVENT READ getCategoryEvent)
    Q_PROPERTY(QString CATEGORY_MESSAGE READ getCategoryMessage)

    Q_PROPERTY(QString CATEGORY_PROGRESS READ getCategoryProgress)
    Q_PROPERTY(QString CATEGORY_PROMO READ getCategoryPromo)
    Q_PROPERTY(QString CATEGORY_RECOMMENDATION READ getCategoryRecommendation)

    Q_PROPERTY(QString CATEGORY_REMINDER READ getCategoryReminder)
    Q_PROPERTY(QString CATEGORY_SERVICE READ getCategoryService)
    Q_PROPERTY(QString CATEGORY_SOCIAL READ getCategorySocial)

    Q_PROPERTY(QString CATEGORY_STATUS READ getCategoryStatus)
    Q_PROPERTY(QString CATEGORY_SYSTEM READ getCategorySystem)
    Q_PROPERTY(QString CATEGORY_TRANSPORT READ getCategoryTransport)

    //----- Notification Priorities ----- //

    Q_PROPERTY(int PRIORITY_MIN READ getPriorityMin)
    Q_PROPERTY(int PRIORITY_LOW READ getPriorityLow)
    Q_PROPERTY(int PRIORITY_DEFAULT READ getPriorityDefault)
    Q_PROPERTY(int PRIORITY_HIGH READ getPriorityHigh)
    Q_PROPERTY(int PRIORITY_MAX READ getPriorityMax)

    //----- Notification Defaults ----- //

    Q_PROPERTY(int DEFAULT_ALL READ getDefaultAll)
    Q_PROPERTY(int DEFAULT_LIGHTS READ getDefaultLights)
    Q_PROPERTY(int DEFAULT_SOUND READ getDefaultSound)
    Q_PROPERTY(int DEFAULT_VIBRATE READ getDefaultVibrate)

    //----- Notification Flags ----- //

    Q_PROPERTY(int FLAG_AUTO_CANCEL READ getFlagAutoCancel)
    Q_PROPERTY(int FLAG_FOREGROUND_SERVICE READ getFlagForegroundService)
    Q_PROPERTY(int FLAG_GROUP_SUMMARY READ getFlagGroupSummary)

    Q_PROPERTY(int FLAG_INSISTENT READ getFlagInsistent)
    Q_PROPERTY(int FLAG_LOCAL_ONLY READ getFlagLocalOnly)
    Q_PROPERTY(int FLAG_NO_CLEAR READ getFlagNoClear)

    Q_PROPERTY(int FLAG_ONGOING_EVENT READ getFlagOngoingEvent)
    Q_PROPERTY(int FLAG_ONLY_ALERT_ONCE READ getFlagOnlyAlertOnce)
    Q_PROPERTY(int FLAG_SHOW_LIGHTS READ getFlagShowLights)

    //----- Notification Visibility ----- //

    Q_PROPERTY(int VISIBILITY_PRIVATE READ getVisibilityPrivate)
    Q_PROPERTY(int VISIBILITY_PUBLIC READ getVisibilityPublic)
    Q_PROPERTY(int VISIBILITY_SECRET READ getVisibilitySecret)

public:
    explicit Notification(QObject *parent = nullptr);
    Notification(Notification &&other);

    /**
     * @brief This will construct a Notification object from the given map. Use the default property names when notifying with a JS map.
     * Only the `text` and `title` properties are mandatory.
     * @param data
     * @return
     */
    static Notification fromVariantMap(const QVariantMap &data);

    /**
     * @brief A valid Notification **must** have the  `title` and `text` properties full.
     * @return
     */
    bool isValid() const;

    void setPriority(int priority);
    int getPriority() const;

    void setCategory(QString category);
    QString getCategory() const;

    void setLedColor(const QColor &color);
    QColor getLedColor() const;

    void setLedOnMS(const int &ms);
    int getLedOnMS() const;

    void setLedOffMS(const int &ms);
    int getLedOffMS() const;

    void setTitle(QString title);
    QString getTitle() const;

    void setText(QString text);
    QString getText() const;

    void setDelay(int delay);
    int getDelay() const;

    void setDate(QDateTime date);
    QDateTime getDate() const;

    void setNotificationTag(QString tag);
    QString getNotificationTag() const;

    int getNotificationID() const;
    void setNotificationID(int id);

    void setSound(QString sound);
    QString getSound() const;

    void setNotificationManagerName(QString name);
    QString getNotificationManagerName() const;

    void setDefaults(int defaults);
    int getDefaults() const;

    void setFlags(int flags);
    int getFlags() const;

    void setVisibility(int visibility);
    int getVisibility() const;

    //----- Notification Categories ----- //

    QString getCategoryAlarm() const;
    QString getCategoryCall() const;
    QString getCategoryEmail() const;

    QString getCategoryError() const;
    QString getCategoryEvent() const;
    QString getCategoryMessage() const;

    QString getCategoryProgress() const;
    QString getCategoryPromo() const;
    QString getCategoryRecommendation() const;

    QString getCategoryReminder() const;
    QString getCategoryService() const;
    QString getCategorySocial() const;

    QString getCategoryStatus() const;
    QString getCategorySystem() const;
    QString getCategoryTransport() const;

    //----- Notification Priorities ----- //

    int getPriorityMin() const;
    int getPriorityLow() const;
    int getPriorityDefault() const;

    int getPriorityHigh() const;
    int getPriorityMax() const;

    //----- Notification Defaults ----- //

    int getDefaultAll() const;
    int getDefaultLights() const;
    int getDefaultSound() const;

    int getDefaultVibrate() const;

    //----- Notification Flags ----- //

    int getFlagAutoCancel() const;
    int getFlagForegroundService() const;
    int getFlagGroupSummary() const;

    int getFlagInsistent() const;
    int getFlagLocalOnly() const;
    int getFlagNoClear() const;

    int getFlagOngoingEvent() const;
    int getFlagOnlyAlertOnce() const;
    int getFlagShowLights() const;

    //----- Notification Visibility ----- //

    int getVisibilityPrivate() const;
    int getVisibilityPublic() const;
    int getVisibilitySecret() const;

signals:
    void priorityChanged();
    void categoryChanged();
    void ledColorChanged();

    void ledOnMSChanged();
    void ledOffMSChanged();
    void titleChanged();

    void textChanged();
    void delayChanged();
    void dateChanged();

    void notificationTagChanged();
    void notificationIDChanged();
    void soundChanged();

    void defaultsChanged();
    void flagsChanged();
    void visibilityChanged();

private:
    /**
     * @brief Acceptable values are:
     * - PRIORITY_MIN: -2
     * - PRIORITY_LOW: -1
     * - PRIORITY_DEFAULT: 0
     * - PRIORITY_HIGH: 1
     * - PRIORITY_MAX: 2
     */
    int m_PriorityFlag;

    /**
     * @brief You can see the full list of categories here: https://developer.android.com/reference/android/support/v4/app/NotificationCompat.html#CATEGORY_ALARM
     */
    QString m_Category;
    QString m_Text,
            m_Title,
            m_NotificationTag,
            m_Sound;
    /**
     * @brief Use this to limit the notification to an instance of the NotificationManager. This is used **only** when a scheduled notification is clicked by the
     * user when the app is closed. If this is set to a valid NotificationManager name, then only that manager will be aware of the notification.
     * If this is left empty, every NotificationManager will receive the signal.
     */
    QString m_TargetNotificationManagerName;

    QColor m_LedColor;

    int m_LedOnMS,
        m_LedOffMS,
        m_DelayMS;

    int m_Defaults,
        m_Flags,
        m_Visibility,
        m_NotificationID;

    QDateTime m_Date;
};

}
