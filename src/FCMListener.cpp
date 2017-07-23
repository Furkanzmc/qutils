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
    if (instance) {
        LOG("An App instance already exists.");
        m_FirebaseApp = instance;
    }
    else {
#ifdef Q_OS_ANDROID
        QAndroidJniObject jniActivity = QtAndroid::androidActivity();
        m_FirebaseApp = firebase::App::Create(m_JNIEnv, jniActivity.object<jobject>());
#else
        m_FirebaseApp = firebase::App::Create();
#endif // Q_OS_ANDROID
    }

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
    firebase::messaging::Message m(message); // Crashes here.
    message.data.size(); // But this does not crash.
}

}
