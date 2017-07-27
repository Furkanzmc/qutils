#include "qutils/FCMListener.h"
#include <sstream>
#include <thread>
// Firebase
#include "firebase/app.h"
// Qt
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#endif // Q_OS_ANDROID
#include <QDebug>

#include <android/log.h>

#define LOG(msg) qDebug() << "[INFO] " << msg

namespace zmc
{

FCMListener *FCMListener::m_Instance = nullptr;

static firebase::InitResult _InitializeMessaging(firebase::App *app, void *)
{
    LOG("Try to initialize Firebase Messaging");
    std::stringstream ss;
    ss << std::this_thread::get_id();
    LOG(QString::fromStdString(ss.str()));
    const firebase::InitResult result = firebase::messaging::Initialize(*app, FCMListener::getInstance());
    if (result == firebase::InitResult::kInitResultSuccess) {
        LOG("Firebase Messaging is initialized.");
    }

    return result;
}

FCMListener::FCMListener()
    : m_FirebaseApp(nullptr)
#ifdef Q_OS_ANDROID
    , m_JNIEnv()
#endif // Q_OS_ANDROID
    , m_Mutex()
{

}

FCMListener *FCMListener::getInstance()
{
    if (m_Instance == nullptr) {
        m_Instance = new FCMListener();
    }

    return m_Instance;
}

void FCMListener::initializeMessaging()
{
    LOG("Initializing Firebase module");
    firebase::App *instance = firebase::App::GetInstance();
    LOG("1_1");
    if (instance) {
        LOG("1_2");
        LOG("An App instance already exists.");
        m_FirebaseApp = instance;
    }
    else {
        LOG("1_3");
#ifdef Q_OS_ANDROID
        QAndroidJniObject jniActivity = QtAndroid::androidActivity();
        LOG("1_4");
        m_FirebaseApp = firebase::App::Create(m_JNIEnv, jniActivity.object<jobject>());
        LOG("1_5");
#else
        m_FirebaseApp = firebase::App::Create();
#endif // Q_OS_ANDROID
    }

    LOG("1_6");
    std::stringstream ss;
    ss << std::this_thread::get_id();
    LOG(QString::fromStdString(ss.str()));
    m_ModuleInitializer.Initialize(m_FirebaseApp, nullptr, _InitializeMessaging);
    LOG("Module initialized. Waiting on messaging initialization");

}

void FCMListener::OnTokenReceived(const char *token)
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    LOG(QString::fromStdString(ss.str()));
    LOG("Token received: [" << token << "]");
}

void FCMListener::OnMessage(const firebase::messaging::Message &message)
{
    LOG("OnMessage CALLED!!");

    std::lock_guard<std::recursive_mutex> lock(m_Mutex);
    const firebase::messaging::Message m(message); // Crashes here.

//    std::stringstream ss;
//    ss << std::this_thread::get_id();
//    LOG("THREAD ID: " << QString::fromStdString(ss.str()));
//    LOG("MESSAGE POINTER: " << &message);

    LOG(message.data.size()); // But this does not crash.
//    LOG("QString::fromStdString(message.error_description");
//    message.error_description.c_str();
//    LOG("message.data.begin();");
//    message.data.begin();
//    LOG("message.message_type");
//    m_MessageType = message.message_type.c_str();
//    LOG("message.message_type 2");
//    LOG(QString::fromLocal8Bit(m_MessageType));
}

}
