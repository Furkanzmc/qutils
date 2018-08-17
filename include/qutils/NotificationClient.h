#pragma once
// Qt
#include <QObject>
#include <QVariantMap>
// QtFirebase
#if FCM_ENABLED && defined(Q_OS_ANDROID)
    #include "QtFirebase/src/qtfirebasemessaging.h"
    class QtFirebaseMessaging;
#endif // FCM_ENABLED

namespace zmc
{

// Forward Declerations
#ifdef Q_OS_IOS
    class iOSNativeUtils;
#endif // Q_OS_IOS
class Notification;

using NotificationQueueMember = std::tuple<QString/*tag*/, int/*id*/, QString/*managerName*/, QString/*payload*/, bool/*isTapped*/>;
using NotificationQueue = std::vector<NotificationQueueMember>;

class NotificationClient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap payload READ payload CONSTANT)
    Q_PROPERTY(QString token READ token CONSTANT)

public:
    explicit NotificationClient(QObject *parent = 0);
    ~NotificationClient();

    /**
     * @brief If an instance with the given tag and ID exists, returns the instance and removes it from the map.
     * @param notificationTag
     * @param notificationID
     * @return
     */
    static NotificationClient *getInstance(QString notificationTag, int notificationID);

    /**
     * @brief Schedule notification with a Notification object.
     * @note the `zmc` namespace is used in the function definition and implementation to avoid `Unknown method parameter type Notification *` error.
     * @param notification
     */
    Q_INVOKABLE void scheduleNotification(zmc::Notification *notification);
    Q_INVOKABLE void schedule(QVariantMap data);

    /**
     * @brief Returns an available ID and increases the m_NotificationID
     * @return int
     */
    Q_INVOKABLE int getNextID() const;

    /**
     * @brief Returns the FCM token if available. If it is not yet generated, or If FCM is disabled, returns an empty string.
     * @return QString
     */
    Q_INVOKABLE QString getFCMToken() const;

    void emitNotificationReceivedSignal(QString payload);
    void emitNotificationTappedSignal(QString payload);

    /**
     * @brief Notification queue is used when the app receives a notification while it is closed and the user opened the app by clicking on the notification.
     * When the app launches, the JNI_OnLoad function is called and the native functions are registered. After that process, the `notificationReceived` native
     * function is called from the Java side. But at that point, we do not yet have the Qt application and the components fully loaded, so we have to wait until
     * it is fully loaded to be able to call the `notificationReceived()` signal. So, this is why I'm using the queue. NotificationManager is not a singleton
     * class, so there can be multiple instances of the class. When the @ref schedule() or @ref scheduleNotification() functions are used, only the firing
     * NotificationManager will be signaled when the notification arrives. But if the scheduled notification arrives after the app is closed, you can set the
     * `objectName` property of the `NotificationManager` to make sure that only the NotificationManager with the same name gets signaled. If the
     * `objectName` property is empty, then all of the `NotificationManager`s will be signaled when the app finishes loading.
     * @param tup
     */
    static void addNotifiationQueue(const NotificationQueueMember &tup);

    static void emitFCMTokenReceivedSignal(const QString &token);

    /*!
     * \property NotificationClient::payload
     * \brief The payload from the latest notification.
     * \return QVariantMap
     */
    QVariantMap payload() const;

    /*!
     * \property NotificationClient::token
     * \brief Token from the push notification service.
     * \return QString
     */
    QString token() const;

signals:
    void notificationReceived();
    void notificationTapped();
    void fcmTokenReceived();

private:
    static int m_NotificationID;
    static std::vector<std::pair<std::pair<QString, int>, NotificationClient *>> m_Clients;
    static NotificationQueue m_NotificationQueue;

    /*!
     * \variable static QMap<int, CacheManager *> m_Instances
     * \brief This variable is used to keep track of all the instances so that we can send signals to all of them.
     */
    static QMap<int, NotificationClient *> m_Instances;

    /*!
     * \variable static unsigned int m_NextInstanceID
     * \brief Used to prevent key clashes in m_Instances.
     */
    static unsigned int m_NextInstanceID;
#if FCM_ENABLED == 1
    static QString m_FCMToken;
#endif // FCM_ENABLED == 1

    /*!
     * \variable const int m_InstanceIndex
     * \brief This variable is initilized in the constructor to the size value of m_Instances.
     *
     * This is used to identify the current instance in m_Instances.
     */
    const unsigned int m_InstanceIndex;
#ifdef Q_OS_IOS
    iOSNativeUtils *m_iOSNative;
#endif // Q_OS_IOS

#if FCM_ENABLED && defined(Q_OS_ANDROID)
    QtFirebaseMessaging *m_FCMMessaging;
#endif // FCM_ENABLED

    QVariantMap m_LastPayload;

private:
    void setNotificationProperties(const Notification *notification);
    void processQueue();
#ifdef Q_OS_ANDROID
    void setNotificationPropertiesAndroid(const Notification *notification);
    void scheduleNotificationAndroid(zmc::Notification *notification);
#endif // Q_OS_ANDROID

#ifdef Q_OS_IOS
    void setNotificationPropertiesIOS(const Notification *notification);
    void scheduleNotificationIOS(zmc::Notification *notification);
#endif // Q_OS_IOS

#if FCM_ENABLED && defined(Q_OS_ANDROID)
    void onMessageReceived();
    void onTokenChanged();
#endif // FCM_ENABLED
};

}
