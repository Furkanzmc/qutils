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

    Q_PROPERTY(QString payload READ getPayload WRITE setPayload NOTIFY dataChanged)

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

public:
    enum class Priority {
        Minimum = -2,
        Low = -1,
        Default = 0,
        High = 1,
        Max = 2
    };
    Q_ENUM(Priority)

    enum class Defaults {
        DefaultAll = -1,
        DefaultLights = 4,
        DefaultSound = 1,
        DefaultVibrate = 2
    };
    Q_ENUM(Defaults)

    enum class Flags {
        AutoCancel = 16,
        ForegroundService = 64,
        GroupSummary = 512,
        Insistent = 4,
        LocalOnly = 256,
        NoClear = 32,
        OngoingEvent = 2,
        OnlyAlertOnce = 8,
        ShowLights = 1
    };
    Q_ENUM(Flags)

    enum class Visibility {
        Private = 0,
        Public = 1,
        Secret = -1
    };
    Q_ENUM(Visibility)

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

    QString getPayload() const;
    void setPayload(const QString &data);

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

    void dataChanged();

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
            m_Sound,
            m_Payload;
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
