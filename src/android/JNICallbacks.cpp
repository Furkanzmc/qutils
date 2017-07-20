#include "qutils/android/JNICallbacks.h"
// Local
#include "qutils/android/AndroidUtils.h"
#include "qutils/Macros.h"

#define ANDROID_UTILS_CLASS "org/zmc/qutils/AndroidUtils"

static const JNINativeMethod JAVA_CALLBACK_METHODS[] = {
    {
        "notificationReceived", // const char* function name;
        "(Ljava/lang/String;ILjava/lang/String;)V", // const char* function signature
        (void *)JNICallbacks::notificationReceivedCallback // function pointer
    },
    {
        "backButtonPressed", // const char* function name;
        "()V", // const char* function signature
        (void *)JNICallbacks::backButtonPressedCallback // function pointer
    },
    {
        "menuButtonPressed", // const char* function name;
        "()V", // const char* function signature
        (void *)JNICallbacks::menuButtonPressedCallback // function pointer
    },
    {
        "alertDialogClicked", // const char* function name;
        "(I)V", // const char* function signature
        (void *)JNICallbacks::alertDialogClickedCallback // function pointer
    },
    {
        "datePicked", // const char* function name;
        "(III)V", // const char* function signature
        (void *)JNICallbacks::datePickedCallback // function pointer
    },
    {
        "timePicked", // const char* function name;
        "(II)V", // const char* function signature
        (void *)JNICallbacks::timePickedCallback // function pointer
    },
    {
        "cameraCaptured", // const char* function name;
        "(Ljava/lang/String;)V", // const char* function signature
        (void *)JNICallbacks::cameraCapturedCallback // function pointer
    },
    {
        "cameraCaptureCancelled", // const char* function name;
        "()V", // const char* function signature
        (void *)JNICallbacks::cameraCaptureCancelledCallback // function pointer
    },
    {
        "fileSelected", // const char* function name;
        "(Ljava/lang/String;)V", // const char* function signature
        (void *)JNICallbacks::fileSelectedCallback // function pointer
    },
    {
        "fileSelectionCancelled", // const char* function name;
        "()V", // const char* function signature
        (void *)JNICallbacks::fileSelectionCancelledCallback // function pointer
    },
    {
        "keyboardHeightChanged", // const char* function name;
        "(I)V", // const char* function signature
        (void *)JNICallbacks::keyboardHeightChangedCallback // function pointer
    },
    {
        "fcmTokenReceived", // const char* function name;
        "(Ljava/lang/String;)V", // const char* function signature
        (void *)JNICallbacks::fcmTokenReceived // function pointer
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
    jclass javaClass = env->FindClass("org/zmc/qutils/CppCallbacks");
    if (!javaClass) {
        return JNI_ERR;
    }

    // Register our native methods
    if (env->RegisterNatives(javaClass, JAVA_CALLBACK_METHODS, sizeof(JAVA_CALLBACK_METHODS) / sizeof(JAVA_CALLBACK_METHODS[0])) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

JNICallbacks::JNICallbacks()
{

}

void JNICallbacks::notificationReceivedCallback(JNIEnv */*env*/, jobject /*obj*/, jstring jtag, jint id, jstring jnotificationManagerName)
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

void JNICallbacks::backButtonPressedCallback(JNIEnv */*env*/, jobject /*obj*/)
{
    zmc::AndroidUtils::emitButtonPressedSignals(true, false);
}

void JNICallbacks::menuButtonPressedCallback(JNIEnv */*env*/, jobject /*obj*/)
{
    zmc::AndroidUtils::emitButtonPressedSignals(false, true);
}

void JNICallbacks::alertDialogClickedCallback(JNIEnv */*env*/, jobject /*obj*/, jint buttonIndex)
{
    zmc::AndroidUtils::emitAlertDialogClickedSignals(buttonIndex);
}

void JNICallbacks::datePickedCallback(JNIEnv */*env*/, jobject /*obj*/, jint year, jint month, jint day)
{
    zmc::AndroidUtils::emitDatePickedSignals(year, month, day);
}

void JNICallbacks::timePickedCallback(JNIEnv */*env*/, jobject /*obj*/, jint hourOfDay, jint minute)
{
    zmc::AndroidUtils::emitTimePickedSignals(hourOfDay, minute);
}

void JNICallbacks::cameraCapturedCallback(JNIEnv */*env*/, jobject /*obj*/, jstring capturePath)
{
    QAndroidJniObject jniStr(capturePath);
    zmc::AndroidUtils::emitCameraCapturedSignals(jniStr.toString());
}

void JNICallbacks::fileSelectedCallback(JNIEnv */*env*/, jobject /*obj*/, jstring filePath)
{
    QAndroidJniObject jniStr(filePath);
    zmc::AndroidUtils::emitFileSelectedSignals(jniStr.toString());
}

void JNICallbacks::cameraCaptureCancelledCallback(JNIEnv */*env*/, jobject /*obj*/)
{
    zmc::AndroidUtils::emitCameraCaptureCancelledSignals();
}

void JNICallbacks::fileSelectionCancelledCallback(JNIEnv */*env*/, jobject /*obj*/)
{
    zmc::AndroidUtils::emitFileSelectionCancelledSignals();
}

void JNICallbacks::keyboardHeightChangedCallback(JNIEnv */*env*/, jobject /*obj*/, jint keyboardHeight)
{
    zmc::AndroidUtils::emitKeyboardHeightChangedSignals(keyboardHeight);
}

void JNICallbacks::fcmTokenReceived(JNIEnv *, jobject, jstring jniToken)
{
    const QString token = QAndroidJniObject(jniToken).toString();
    LOG("Token: " << token);
}
