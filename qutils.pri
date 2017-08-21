CONFIG += c++11
QT += sql

contains(CONFIG, QUTILS_NO_MULTIMEDIA) {
    message("[qutils] Multimedia is disabled in qutils")
    QUTILS_NO_MULTIMEDIA=false
}
else {
    QUTILS_NO_MULTIMEDIA=true
    QT += multimedia
}

FCM_ENABLED=false
contains(CONFIG, ENABLE_FCM) {
    message("[qutils] Firebase Cloud Messageing is enabled.")
    FCM_ENABLED=true
}
else {
    message("[qutils] Firebase Cloud Messageing is NOT enabled.")
}

android {
    QT += androidextras

    isEqual(FCM_ENABLED, true) {
        DEFINES += FCM_ENABLED=1
        OTHER_FILES += \
            $$PWD/android/src/org/zmc/qutils/notification/QutilsFirebaseInstanceIDService.java \
            $$PWD/android/src/org/zmc/qutils/notification/QutilsFirebaseMessagingService.java
    }
    else {
        DEFINES += FCM_ENABLED=0
    }

    OTHER_FILES += \
        $$PWD/android/src/org/zmc/qutils/notification/NotificationClient.java \
        $$PWD/android/src/org/zmc/qutils/notification/NotificationReceiver.java \
        $$PWD/android/src/org/zmc/qutils/CppCallbacks.java \
        $$PWD/android/src/org/zmc/qutils/QutilsActivity.java \
        $$PWD/android/src/org/zmc/qutils/AndroidUtils.java \
        $$PWD/android/src/org/zmc/qutils/DatePickerFragment.java \
        $$PWD/android/src/org/zmc/qutils/TimePickerFragment.java

    HEADERS += \
        $$PWD/include/qutils/android/Notification_Android.h \
        $$PWD/include/qutils/android/NotificationClient_Android.h \
        $$PWD/include/qutils/android/AndroidUtils.h \
        $$PWD/include/qutils/android/JNICallbacks.h

    SOURCES += \
        $$PWD/src/android/Notification_Android.cpp \
        $$PWD/src/android/NotificationClient_Android.cpp \
        $$PWD/src/android/AndroidUtils.cpp \
        $$PWD/src/android/JNICallbacks.cpp

    INCLUDEPATH += $$PWD/include/android
}

ios {
    OBJECTIVE_HEADERS += \
        $$PWD/include/qutils/ios/iOSNativeUtils.h

    OBJECTIVE_SOURCES += \
        $$PWD/src/ios/iOSNativeUtils.mm

    HEADERS += \
        $$PWD/include/qutils/ios/iOSUtils.h

    SOURCES += \
        $$PWD/src/ios/iOSUtils.cpp
}

HEADERS += \
    $$PWD/include/qutils/ScreenHelper.h \
    $$PWD/include/qutils/QMLRefresh.h \
    $$PWD/include/qutils/NotificationClient.h \
    $$PWD/include/qutils/Notification.h \
    $$PWD/include/qutils/Macros.h \
    $$PWD/include/qutils/TranslationHelper.h \
    $$PWD/include/qutils/NativeUtils.h \
    $$PWD/include/qutils/SqliteManager.h \
    $$PWD/include/qutils/SettingsManager.h \
    $$PWD/include/qutils/CacheManager.h \
    $$PWD/include/qutils/Network/NetworkManager.h \
    $$PWD/include/qutils/Network/DownloadManager.h \
    $$PWD/include/qutils/Network/HttpCodes.h \
    $$PWD/include/qutils/JsonUtils.h \
    $$PWD/include/qutils/SignalManager.h \
    $$PWD/include/qutils/DateManager.h \
    $$PWD/include/qutils/QutilsLog.h \
    $$PWD/include/qutils/FileUtils.h \
    $$PWD/include/qutils/CppTypes.h

SOURCES += \
    $$PWD/src/ScreenHelper.cpp \
    $$PWD/src/TranslationHelper.cpp \
    $$PWD/src/NativeUtils.cpp \
    $$PWD/src/SqliteManager.cpp \
    $$PWD/src/SettingsManager.cpp \
    $$PWD/src/CacheManager.cpp \
    $$PWD/src/Network/NetworkManager.cpp \
    $$PWD/src/Network/DownloadManager.cpp \
    $$PWD/src/JsonUtils.cpp \
    $$PWD/src/SignalManager.cpp \
    $$PWD/src/DateManager.cpp \
    $$PWD/src/QMLRefresh.cpp \
    $$PWD/src/QutilsLog.cpp \
    $$PWD/src/FileUtils.cpp

if (!QUTILS_NO_MULTIMEDIA) {
    HEADERS += \
        $$PWD/include/qutils/AudioRecorder.h

    SOURCES += \
        $$PWD/src/AudioRecorder.cpp
}

INCLUDEPATH += $$PWD/include

DISTFILES += \
    $$PWD/android/src/org/zmc/qutils/Constants.java
