#pragma once
// Qt
#include <QObject>
#include <QColor>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#endif // Q_OS_ANDROID
// Local
#include "zmc/android/NotificationClient_Android.h"

static void notificationReceivedCallback(JNIEnv */*env*/, jobject /*obj*/, jstring jtag, jint id, jstring jnotificationManagerName)
{
    const QString tag = QAndroidJniObject(jtag).toString();
    const QString managerName = QAndroidJniObject(jnotificationManagerName).toString();
    zmc::NotificationClient *client = zmc::NotificationClient::getInstance(tag, id);
    if (client) {
        client->emitNotificationReceivedSignal(tag, id);
    }
    else {
        zmc::NotificationClient::addNotifiationQueue(std::make_tuple(tag, id, managerName));
    }
}

namespace zmc
{

class AndroidUtils : public QObject
{
    Q_OBJECT
public:
    explicit AndroidUtils(QObject *parent = 0);

    Q_INVOKABLE void setStatusBarColor(QColor color);
    Q_INVOKABLE void setStatusBarVisible(bool visible);
    Q_INVOKABLE void setImmersiveMode(bool visible);

signals:

public slots:
};

}
