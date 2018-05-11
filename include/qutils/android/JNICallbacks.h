#pragma once
// Qt
#include <QAndroidJniEnvironment>

class JNICallbacks
{
public:
    JNICallbacks();

    static void notificationReceivedCallback(
        JNIEnv */*env*/,
        jobject /*obj*/,
        jstring jtag,
        jint id,
        jstring jnotificationManagerName,
        jstring notificationPayload
    );
    static void notificationTappedCallback(
        JNIEnv */*env*/,
        jobject /*obj*/,
        jstring jtag,
        jint id,
        jstring jnotificationManagerName,
        jstring notificationPayload
    );
    static void backButtonPressedCallback(JNIEnv */*env*/, jobject /*obj*/);

    static void menuButtonPressedCallback(JNIEnv */*env*/, jobject /*obj*/);
    static void alertDialogClickedCallback(JNIEnv */*env*/, jobject /*obj*/, jint buttonIndex);
    static void datePickedCallback(
        JNIEnv */*env*/,
        jobject /*obj*/,
        jint year,
        jint month,
        jint day
    );

    static void timePickedCallback(
        JNIEnv */*env*/,
        jobject /*obj*/,
        jint hourOfDay,
        jint minute
    );
    static void cameraCapturedCallback(JNIEnv */*env*/, jobject /*obj*/, jstring capturePath);
    static void fileSelectedCallback(JNIEnv */*env*/, jobject /*obj*/, jstring filePath);

    static void cameraCaptureCancelledCallback(JNIEnv */*env*/, jobject /*obj*/);
    static void fileSelectionCancelledCallback(JNIEnv */*env*/, jobject /*obj*/);
    static void keyboardHeightChangedCallback(JNIEnv */*env*/, jobject /*obj*/, jint keyboardHeight);

    static void fcmTokenReceived(JNIEnv */*env*/, jobject /*obj*/, jstring jniToken);
    static void openedWithURL(JNIEnv */*env*/, jobject /*obj*/, jstring jniURL);
};
