#include "qutils/ios/NotificationClient_iOS.h"
// std
#include <functional>
// Qt
#include <QGuiApplication>
#include <QThread>
#include <QTimer>
#include <QDateTime>
#include <QMetaMethod>
// Local
#include "qutils/Macros.h"
#include "qutils/ios/iOSNativeUtils.h"
#include "qutils/ios/Notification_iOS.h"

using ClientPair = std::pair<std::pair<QString, int>, zmc::NotificationClient *>;
using ClientsList = std::vector<ClientPair>;

#define NOTIFICATION_CLIENT_CLASS "org/zmc/qutils/notification/NotificationClient"

namespace zmc
{

int NotificationClient::m_NotificationID = -1;
ClientsList NotificationClient::m_Clients = ClientsList();
NotificationQueue NotificationClient::m_NotificationQueue = NotificationQueue();

QList<NotificationClient *> NotificationClient::m_Instances = QList<NotificationClient *>();
#if FCM_ENABLED == 1
QString NotificationClient::m_FCMToken = "";
#endif // FCM_ENABLED == 1

NotificationClient::NotificationClient(QObject *parent)
    : QObject(parent)
    , m_InstanceIndex(m_Instances.size())
    , m_iOSNative(new iOSNativeUtils())
{
    QTimer::singleShot(1, std::bind(&NotificationClient::processQueue, this));
    m_Instances.push_back(this);

#if FCM_ENABLED == 1
    if (m_FCMToken.length() > 0) {
        QTimer::singleShot(1, std::bind(NotificationClient::emitFCMTokenReceivedSignal, m_FCMToken));
        m_FCMToken = "";
    }
#endif // FCM_ENABLED == 1
}

NotificationClient::~NotificationClient()
{
    m_Instances[m_InstanceIndex] = nullptr;
}

void NotificationClient::scheduleNotification(zmc::Notification *notification)
{
    notification->setNotificationManagerName(this->objectName());
    setNotificationProperties(notification);

    qint64 delay = notification->getDelay();

    m_iOSNative->schedulePushNotification(notification->getTitle(), notification->getText(), delay);

//    m_Clients.push_back(std::make_pair(std::make_pair(notification->getNotificationTag(), notificationID), this));
}

void NotificationClient::schedule(QVariantMap data)
{
    Notification notif = Notification::fromVariantMap(data);
    if (notif.isValid()) {
        scheduleNotification(&notif);
    }
}

int NotificationClient::getNextID() const
{
    m_NotificationID++;
    return m_NotificationID;
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

void NotificationClient::addNotifiationQueue(const NotificationQueueMember &tup)
{
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
        for (NotificationClient *client : m_Instances) {
            client->processQueue();
        }

        m_NotificationQueue.pop_back();
    }
}

QString NotificationClient::getFCMToken() const
{
    return m_iOSNative->getFCMToken();
}

void NotificationClient::emitFCMTokenReceivedSignal(const QString &token)
{
    if (m_Instances.size() > 0) {
        for (NotificationClient *client : m_Instances) {
            if (client) {
                client->fcmTokenReceived(token);
            }
        }
    }
#if FCM_ENABLED == 1
    else {
        m_FCMToken = token;
    }
#endif // FCM_ENABLED == 1
}

void NotificationClient::emitNotificationReceivedSignal(QString payload)
{
    emit notificationReceived(payload);
}

void NotificationClient::emitNotificationTappedSignal(QString payload)
{
    emit notificationTapped(payload);
}

void NotificationClient::setNotificationProperties(const Notification *notification)
{
    Q_UNUSED(notification);
    // Set target manager
//    QAndroidJniObject jniManagerName = QAndroidJniObject::fromString(notification->getNotificationManagerName());
//    QAndroidJniObject::callStaticMethod<void>(
//        NOTIFICATION_CLIENT_CLASS,
//        "setTargetNotificationManager",
//        "(Ljava/lang/String;)V",
//        jniManagerName.object<jstring>());
}

void NotificationClient::processQueue()
{
    if (m_NotificationQueue.size() == 0) {
        return;
    }

    QString objectName = this->objectName();
    for (unsigned int index = 0; index < m_NotificationQueue.size(); index++) {
        const NotificationQueueMember &tup = m_NotificationQueue[index];
        const QString payload = std::get<3>(tup);

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
                emit notificationTapped(payload);
            }
            else {
                emit notificationReceived(payload);
            }
        }
    }
}

}
