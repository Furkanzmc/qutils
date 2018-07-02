#include "qutils/NotificationClient.h"
// std
#include <functional>
// Qt
#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QAndroidJniObject>
#endif // Q_OS_ANDROID
#include <QTimer>
#include <QThread>
#include <QDateTime>
#include <QMetaMethod>
#include <QGuiApplication>
// QtFirebase
#if FCM_ENABLED && defined(Q_OS_ANDROID)
    #include "QtFirebase/src/qtfirebasemessaging.h"
#endif // FCM_ENABLED
// Local
#ifdef Q_OS_ANDROID
    #include "qutils/android/JNICallbacks.h"
#endif // Q_OS_ANDROID
#ifdef Q_OS_IOS
    #include "qutils/ios/iOSNativeUtils.h"
#endif // Q_OS_IOS
#include "qutils/Macros.h"
#include "qutils/JsonUtils.h"
#include "qutils/Notification.h"

using ClientPair = std::pair<std::pair<QString, int>, zmc::NotificationClient *>;
using ClientsList = std::vector<ClientPair>;

#ifdef Q_OS_ANDROID
    #define NOTIFICATION_CLIENT_CLASS "org/zmc/qutils/notification/NotificationClient"
#endif // Q_OS_ANDROID

namespace zmc
{

int NotificationClient::m_NotificationID = -1;
ClientsList NotificationClient::m_Clients = ClientsList();
NotificationQueue NotificationClient::m_NotificationQueue = NotificationQueue();

QMap<int, NotificationClient *> NotificationClient::m_Instances = QMap<int, NotificationClient *>();
#if FCM_ENABLED == 1
    QString NotificationClient::m_FCMToken = "";
#endif // FCM_ENABLED == 1

NotificationClient::NotificationClient(QObject *parent)
    : QObject(parent)
    , m_InstanceIndex(m_Instances.size())
#ifdef Q_OS_IOS
    , m_iOSNative(new iOSNativeUtils())
#endif // Q_OS_IOS
#if FCM_ENABLED && defined(Q_OS_ANDROID)
    , m_FCMMessaging(new QtFirebaseMessaging(this))
#endif // FCM_ENABLED
    , m_LastPayload()
{
    QTimer::singleShot(1, std::bind(&NotificationClient::processQueue, this));
    m_Instances.insert(m_InstanceIndex, this);

#if FCM_ENABLED && defined(Q_OS_ANDROID)
    // Call this to connect the signals. QtFirebase is designed to be used with QML.
    m_FCMMessaging->componentComplete();
    connect(m_FCMMessaging, &QtFirebaseMessaging::messageReceived, this, &NotificationClient::onMessageReceived);
    connect(m_FCMMessaging, &QtFirebaseMessaging::tokenChanged, this, &NotificationClient::onTokenChanged);
#endif // FCM_ENABLED
}

NotificationClient::~NotificationClient()
{
    m_Instances.remove(m_InstanceIndex);
#ifdef Q_OS_IOS
    delete m_iOSNative;
    m_iOSNative = nullptr;
#endif // Q_OS_IOS
}

NotificationClient *NotificationClient::getInstance(QString notificationTag, int notificationID)
{
    NotificationClient *instance = nullptr;

    auto foundIt = std::find_if(m_Clients.begin(), m_Clients.end(), [&notificationTag, &notificationID](const ClientPair & inst) {
        return inst.first.first == notificationTag && inst.first.second == notificationID;
    });

    if (foundIt != m_Clients.end()) {
        instance = (*foundIt).second;
        m_Clients.erase(foundIt);
    }
    else {
        LOG("Cannot find the instance for the notification (" << notificationTag << ", " << notificationID << ").");

    }

    return instance;
}

void NotificationClient::scheduleNotification(zmc::Notification *notification)
{
#ifdef Q_OS_MOBILE
#  ifdef Q_OS_ANDROID
    scheduleNotificationAndroid(notification);
#  endif // Q_OS_ANDROID

#  ifdef Q_OS_IOS
    scheduleNotificationIOS(notification);
#  endif // Q_OS_IOS
#else
    Q_UNUSED(notification);
#endif // Q_OS_MOBILE
}

void NotificationClient::schedule(QVariantMap data)
{
#ifdef Q_OS_MOBILE
    Notification notif = Notification::fromVariantMap(data);
    if (notif.isValid()) {
        scheduleNotification(&notif);
    }
#else
    Q_UNUSED(data);
#endif // Q_OS_MOBILE
}

int NotificationClient::getNextID() const
{
#ifdef Q_OS_MOBILE
    m_NotificationID++;
#endif // Q_OS_MOBILE
    return m_NotificationID;
}

QString NotificationClient::getFCMToken() const
{
    QString token = "";
#ifdef Q_OS_IOS
    token = m_iOSNative->getFCMToken();
#endif // Q_OS_IOS

#ifdef Q_OS_ANDROID

#endif // Q_OS_ANDROID
    return token;
}

void NotificationClient::emitNotificationReceivedSignal(QString payload)
{
#ifdef Q_OS_MOBILE
    m_LastPayload = JsonUtils::toVariantMap(payload);
    emit notificationReceived();
#else
    Q_UNUSED(payload);
#endif // Q_OS_MOBILE
}

void NotificationClient::emitNotificationTappedSignal(QString payload)
{
#ifdef Q_OS_MOBILE
    m_LastPayload = JsonUtils::toVariantMap(payload);
    emit notificationTapped();
#else
    Q_UNUSED(payload);
#endif // Q_OS_MOBILE
}

void NotificationClient::addNotifiationQueue(const NotificationQueueMember &tup)
{
#ifdef Q_OS_MOBILE
    auto foundIt = std::find_if(m_NotificationQueue.begin(), m_NotificationQueue.end(), [&tup](const NotificationQueueMember & inTuple) {
        return std::get<0>(tup) == std::get<0>(inTuple) && std::get<1>(tup) == std::get<1>(inTuple)
            && std::get<2>(tup) == std::get<2>(inTuple) && std::get<4>(tup) == std::get<4>(inTuple);
    });

    // Do not add the same notification twice.
    if (foundIt == m_NotificationQueue.end()) {
        m_NotificationQueue.push_back(tup);
    }

    // Let every instance process and then remove the tuple from the queue.
    if (m_Instances.size() > 0) {
        auto begin = m_Instances.begin();
        auto end = m_Instances.end();
        for (auto it = begin; it != end; it++) {
            NotificationClient *client = it.value();
            client->processQueue();
        }

        m_NotificationQueue.pop_back();
    }
#else
    Q_UNUSED(tup);
#endif // Q_OS_MOBILE
}

void NotificationClient::emitFCMTokenReceivedSignal(const QString &token)
{
#ifdef Q_OS_MOBILE
    Q_UNUSED(token);
    if (m_Instances.size() > 0) {
        auto begin = m_Instances.begin();
        auto end = m_Instances.end();
        for (auto it = begin; it != end; it++) {
            NotificationClient *client = it.value();
            if (client) {
                QMetaObject::invokeMethod(client, std::bind(&NotificationClient::fcmTokenReceived, client), Qt::QueuedConnection);
            }
        }
    }
#if FCM_ENABLED == 1
    else {
        m_FCMToken = token;
    }
#endif // FCM_ENABLED == 1
#else
    Q_UNUSED(token);
#endif // Q_OS_MOBILE
}

QVariantMap NotificationClient::payload() const
{
    return m_LastPayload;
}

QString NotificationClient::token() const
{
    QString tk = "";
#if FCM_ENABLED && defined(Q_OS_ANDROID)
    tk = m_FCMMessaging->token();
#elif defined(Q_OS_IOS)
    tk = m_iOSNative->getFCMToken();
#endif // FCM_ENABLED

    return tk;
}

void NotificationClient::setNotificationProperties(const Notification *notification)
{
#ifdef Q_OS_MOBILE
#ifdef Q_OS_ANDROID
    setNotificationPropertiesAndroid(notification);
#endif // Q_OS_ANDROID

#ifdef Q_OS_IOS
    setNotificationPropertiesIOS(notification);
#endif // Q_OS_IOS
#else
    Q_UNUSED(notification);
#endif // Q_OS_MOBILE
}

void NotificationClient::processQueue()
{
    if (m_NotificationQueue.size() == 0) {
        return;
    }

    QString objectName = this->objectName();
    for (unsigned int index = 0; index < m_NotificationQueue.size(); index++) {
        const NotificationQueueMember &tup = m_NotificationQueue[index];
        m_LastPayload = JsonUtils::toVariantMap(std::get<3>(tup));

        const QString managerName = std::get<2>(tup);
        bool shouldNotify = false;

        if ((objectName.length() > 0 && objectName == managerName)) {
            shouldNotify = true;
        }
        else if (managerName.length() == 0) {
            shouldNotify = true;
        }

        if (shouldNotify) {
            if (std::get<4>(tup) == true) {
                emit notificationTapped();
            }
            else {
                emit notificationReceived();
            }
        }
    }
}

#ifdef Q_OS_ANDROID

void NotificationClient::setNotificationPropertiesAndroid(const Notification *notification)
{
    Q_UNUSED(notification);
}

void NotificationClient::scheduleNotificationAndroid(Notification *notification)
{
    Q_UNUSED(notification);
}

#endif // Q_OS_ANDROID

#ifdef Q_OS_IOS

void NotificationClient::setNotificationPropertiesIOS(const Notification *notification)
{
    Q_UNUSED(notification);
}

void NotificationClient::scheduleNotificationIOS(zmc::Notification *notification)
{
    notification->setNotificationManagerName(this->objectName());
    setNotificationProperties(notification);

    qint64 delay = notification->getDelay();
    m_iOSNative->schedulePushNotification(notification->getTitle(), notification->getText(), static_cast<int>(delay));
    m_Clients.push_back(std::make_pair(std::make_pair(notification->getNotificationTag(), -1), this));
}

#endif // Q_OS_IOS

#if FCM_ENABLED && defined(Q_OS_ANDROID)
void NotificationClient::onMessageReceived()
{
    m_LastPayload = m_FCMMessaging->data();
    emit notificationReceived();
}

void NotificationClient::onTokenChanged()
{
    emit fcmTokenReceived();
}
#endif // FCM_ENABLED

}
