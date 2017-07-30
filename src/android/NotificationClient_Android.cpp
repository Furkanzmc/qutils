#include "qutils/android/NotificationClient_Android.h"
// std
#include <functional>
// Qt
#include <QtAndroidExtras/QAndroidJniObject>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
// Local
#include "qutils/android/Notification_Android.h"
#include "qutils/android/JNICallbacks.h"
#include "qutils/Macros.h"

using ClientPair = std::pair<std::pair<QString, int>, zmc::NotificationClient *>;
using ClientsList = std::vector<ClientPair>;
using NotificationQueue = std::vector<std::tuple<QString, int, QString, QString>>;
using NotificationQueueMember = std::tuple<QString, int, QString, QString>;

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
{
    /*
     * This is to execute the `processQueue` function with the next cycle.
     * This is used to make sure that NativeUtils and AndroidUtils can catch the emitted signal.
     */
    QTimer::singleShot(1, std::bind(&NotificationClient::processQueue, this));

    m_Instances.push_back(this);

    if (m_FCMToken.length() > 0) {
        QTimer::singleShot(1, std::bind(NotificationClient::emitFCMTokenReceivedSignal, m_FCMToken));
        m_FCMToken = "";
    }
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
    const QDateTime date = notification->getDate();
    if (date.isValid()) {
        const QDateTime current = QDateTime::currentDateTime();
        delay = current.msecsTo(date);
    }

    int notificationID = QAndroidJniObject::callStaticMethod<jint>(
                             NOTIFICATION_CLIENT_CLASS,
                             "getLastNotificationID",
                             "()I");

    m_Clients.push_back(std::make_pair(std::make_pair(notification->getNotificationTag(), notificationID), this));

    QAndroidJniObject javaNotificationTitle = QAndroidJniObject::fromString(notification->getTitle());
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(notification->getText());
    QAndroidJniObject javaNotificationData = QAndroidJniObject::fromString(notification->getPayload());
    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "notify",
        "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)V",
        javaNotification.object<jstring>(),
        javaNotificationTitle.object<jstring>(),
        javaNotificationData.object<jstring>(),
        delay);
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

void NotificationClient::addNotifiationQueue(const std::tuple<QString, int, QString, QString> &tup, bool isTapped)
{
    auto foundIt = std::find_if(m_NotificationQueue.begin(), m_NotificationQueue.end(), [&tup](const NotificationQueueMember & inTuple) {
        return std::get<0>(tup) == std::get<0>(inTuple) && std::get<1>(tup) == std::get<1>(inTuple) && std::get<2>(tup) == std::get<2>(inTuple);
    });

    // Do not add the same notification twice.
    if (foundIt == m_NotificationQueue.end()) {
        m_NotificationQueue.push_back(tup);
    }

    if (m_Instances.size() > 0) {
        for (NotificationClient *client : m_Instances) {
            client->processQueue();
        }
    }
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
    else {
        m_FCMToken = token;
    }
}

void NotificationClient::emitNotificationReceivedSignal(QString payload)
{
    LOG_JNI("NOTIFICATION EMITTED!!!");
    emit notificationReceived(payload);
}

void NotificationClient::setNotificationProperties(const Notification *notification)
{
    // Set defaults
    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "setDefaults",
        "(I)V",
        notification->getDefaults());

    // Set flags
    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "setFlags",
        "(I)V",
        notification->getFlags());

    // Set priority
    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "setPriority",
        "(I)V",
        notification->getPriority());

    // Set category
    QAndroidJniObject jniCategory = QAndroidJniObject::fromString(notification->getCategory());
    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "setCategory",
        "(Ljava/lang/String;)V",
        jniCategory.object<jstring>());

    // Set led color
    QString ledColor = "";
    if (notification->getLedColor().isValid()) {
        ledColor = notification->getLedColor().name(QColor::NameFormat::HexArgb);
    }

    QAndroidJniObject jniColor = QAndroidJniObject::fromString(ledColor);
    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "setCategory",
        "(Ljava/lang/String;)V",
        jniColor.object<jstring>());

    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "setLedOnMS",
        "(I)V",
        notification->getLedOnMS());

    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "setLedOffMS",
        "(I)V",
        notification->getLedOffMS());

    // Set sound
    QAndroidJniObject jniSound = QAndroidJniObject::fromString(notification->getSound());
    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "setSound",
        "(Ljava/lang/String;)V",
        jniSound.object<jstring>());

    // Set notification tag
    QAndroidJniObject jniNotTag = QAndroidJniObject::fromString(notification->getNotificationTag());
    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "setNotificationTag",
        "(Ljava/lang/String;)V",
        jniNotTag.object<jstring>());

    // Set target manager
    QAndroidJniObject jniManagerName = QAndroidJniObject::fromString(notification->getNotificationManagerName());
    QAndroidJniObject::callStaticMethod<void>(
        NOTIFICATION_CLIENT_CLASS,
        "setTargetNotificationManager",
        "(Ljava/lang/String;)V",
        jniManagerName.object<jstring>());
}

void NotificationClient::processQueue()
{
    if (m_NotificationQueue.size() == 0) {
        return;
    }

    QString objectName = this->objectName();
    for (unsigned int index = 0; index < m_NotificationQueue.size(); index++) {
        const NotificationQueueMember tup = m_NotificationQueue.at(index);
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
            m_NotificationQueue.erase(m_NotificationQueue.begin() + index);
            emit notificationReceived(payload);
        }
    }
}

}
