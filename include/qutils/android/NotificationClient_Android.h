#pragma once
#include <QObject>
#include <QVariantMap>

namespace zmc
{

class Notification;

class NotificationClient : public QObject
{
    Q_OBJECT

public:
    explicit NotificationClient(QObject *parent = 0);

    /**
     * @brief Schedule notification with a Notification object.
     * @note the `zmc` namespace is used in the function definition and implementation to avoid `Unknown method parameter type Notification *` error.
     * @param notification
     */
    Q_INVOKABLE void scheduleNotification(zmc::Notification *notification);
    Q_INVOKABLE void schedule(QVariantMap data);

    /**
     * @brief If an instance with the given tag and ID exists, returns the instance and removes it from the map.
     * @param notificationTag
     * @param notificationID
     * @return
     */
    static NotificationClient *getInstance(QString notificationTag, int notificationID);

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
    static void addNotifiationQueue(const std::tuple<QString, int, QString> &tup);

    void emitNotificationReceivedSignal(QString receivedTag, int receivedID);

    /**
     * @brief Returns an available ID and increases the m_NotificationID
     * @return
     */
    Q_INVOKABLE int getNextID() const;

signals:
    void notificationReceived(QString receivedTag, int receivedID);

private:
    static int m_NotificationID;
    static std::vector<std::pair<std::pair<QString, int>, NotificationClient *>> m_Clients;
    static std::vector<std::tuple<QString, int, QString>> m_NotificationQueue;

private:
#ifdef Q_OS_ANDROID
    void setNotificationProperties(const Notification *notification);
#endif // Q_OS_ANDROID

    void processQueue();
};

}
