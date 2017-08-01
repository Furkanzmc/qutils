#pragma once
#include "qutils/Macros.h"

#ifdef Q_OS_ANDROID
#include "android/NotificationClient_Android.h"
#endif // Q_OS_ANDROID

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
namespace zmc
{

class NotificationClient : public QObject
{
    Q_OBJECT

public:
    NotificationClient()
    {

    }

    Q_INVOKABLE void schedule(QVariantMap data)
    {
        Q_UNUSED(data);
    }

signals:
    void notificationReceived(const QString &payload);
    void notificationTapped(const QString &payload);
    void fcmTokenReceived(const QString &token);
};

}
#endif // !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
