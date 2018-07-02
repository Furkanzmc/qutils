#pragma once
// Qt
#include <QAndroidJniEnvironment>

class JNICallbacks
{
public:
    JNICallbacks();

    static void backButtonPressedCallback(JNIEnv */*env*/, jobject /*obj*/);
    static void menuButtonPressedCallback(JNIEnv */*env*/, jobject /*obj*/);
    static void cameraCapturedCallback(JNIEnv */*env*/, jobject /*obj*/, jstring capturePath);

    static void fileSelectedCallback(JNIEnv */*env*/, jobject /*obj*/, jstring filePath);
    static void cameraCaptureCancelledCallback(JNIEnv */*env*/, jobject /*obj*/);
    static void fileSelectionCancelledCallback(JNIEnv */*env*/, jobject /*obj*/);

    static void keyboardHeightChangedCallback(JNIEnv */*env*/, jobject /*obj*/, jint keyboardHeight);
    static void openedWithURL(JNIEnv */*env*/, jobject /*obj*/, jstring jniURL);
};
