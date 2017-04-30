#include "qutils/android/AndroidUtils.h"
// Qt
#include <QDebug>
#ifdef Q_OS_ANDROID
#include <QtAndroid>

#define ANDROID_UTILS_CLASS "org/zmc/qutils/AndroidUtils"

static const JNINativeMethod JAVA_CALLBACK_METHODS[] = {
    {
        "notificationReceived", // const char* function name;
        "(Ljava/lang/String;ILjava/lang/String;)V", // const char* function signature
        (void *)notificationReceivedCallback // function pointer
    },
    {
        "backButtonPressed", // const char* function name;
        "()V", // const char* function signature
        (void *)backButtonPressedCallback // function pointer
    },
    {
        "menuButtonPressed", // const char* function name;
        "()V", // const char* function signature
        (void *)menuButtonPressedCallback // function pointer
    }
};

// This method is called automatically by Java VM after the .so file is loaded
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void * /*reserved*/)
{
    JNIEnv *env;
    // Get the JNIEnv pointer.
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Search for Java class which declares the native methods
    jclass javaClass = env->FindClass("org/zmc/qutils/notification/CppCallbacks");
    if (!javaClass) {
        return JNI_ERR;
    }

    // Register our native methods
    if (env->RegisterNatives(javaClass, JAVA_CALLBACK_METHODS, sizeof(JAVA_CALLBACK_METHODS) / sizeof(JAVA_CALLBACK_METHODS[0])) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

#endif // Q_OS_ANDROID

namespace zmc
{

std::vector<AndroidUtils *> AndroidUtils::m_Instances = std::vector<AndroidUtils *>();

AndroidUtils::AndroidUtils(QObject *parent)
    : QObject(parent)
{
    m_Instances.push_back(this);
}

void AndroidUtils::setStatusBarColor(QColor color)
{
    auto runnable = [color]() {
        QString ledColor = "";
        if (color.isValid()) {
            ledColor = color.name(QColor::NameFormat::HexArgb);
        }
        QAndroidJniObject jniColor = QAndroidJniObject::fromString(ledColor);
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "setStatusBarColor",
            "(Ljava/lang/String;)V",
            jniColor.object<jstring>());
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::setStatusBarVisible(bool visible)
{
    auto runnable = [visible]() {
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "setStatusBarVisible",
            "(Z)V",
            visible);
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::setImmersiveMode(bool visible)
{
    auto runnable = [visible]() {
        QAndroidJniObject::callStaticMethod<void>(
            ANDROID_UTILS_CLASS,
            "setImmersiveMode",
            "(Z)V",
            visible);
    };

    QtAndroid::runOnAndroidThreadSync(runnable);
}

void AndroidUtils::emitBackButtonPressed()
{
    emit backButtonPressed();
}

void AndroidUtils::emitMenuButtonPressed()
{
    emit menuButtonPressed();
}

void AndroidUtils::emitButtonPressedSignals(bool isBackButton, bool isMenuButton)
{
    for (AndroidUtils *utils : m_Instances) {
        if (isBackButton) {
            utils->emitBackButtonPressed();
        }
        else if (isMenuButton) {
            utils->emitMenuButtonPressed();
        }
    }
}

}
