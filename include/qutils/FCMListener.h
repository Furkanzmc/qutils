#pragma once
// std
#include <mutex>
// Firebase
#include "firebase/messaging.h"
#include "firebase/util.h"
// Local
#include <qsystemdetection.h>
#if (!defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)) && (defined(Q_OS_WIN) || defined(Q_OS_MAC))
#define Q_OS_DESKTOP
#endif // Desktop Platform

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#define Q_OS_MOBILE
#endif // Desktop Platform
// Qt
#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#endif // Q_OS_ANDROID

namespace zmc
{

class FCMListener : public firebase::messaging::Listener
{
public:
    FCMListener();
    static FCMListener *getInstance();
    void initializeMessaging();

    void OnTokenReceived(const char *token) override;
    void OnMessage(const firebase::messaging::Message &message) override;

private:
    static FCMListener *m_Instance;

    const char *m_MessageType;

    firebase::App *m_FirebaseApp;
#ifdef Q_OS_ANDROID
    QAndroidJniEnvironment m_JNIEnv;
#endif // Q_OS_ANDROID
    firebase::ModuleInitializer m_ModuleInitializer;
    std::recursive_mutex m_Mutex;

};

}
