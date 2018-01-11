CONFIG += c++11
QT += sql network

# Available QUTILS_FEATURES options:
# no_photos: Disables the Photos framework on iOS.
# fcm: Enables Firebase messaging integration.
# multimedia: Enables multimedia features.
# for_mobile: Enables mobile related features.

QUTILS_FEATURE_SAFARI_SERVICES = safari_services
QUTILS_FEATURE_FCM = fcm

!isEmpty(QUTILS_APP_NAME) {
    message("[qutils] App name is set to" $$QUTILS_APP_NAME)
    DEFINES += QUTILS_APP_NAME=$$QUTILS_APP_NAME
}
else {
    message("[qutils] App name is not set.")
}

!isEmpty(QUTILS_APP_PACKAGE_NAME) {
    message("[qutils] App package name is set to" $$QUTILS_APP_PACKAGE_NAME)
    DEFINES += QUTILS_APP_PACKAGE_NAME=$$QUTILS_APP_PACKAGE_NAME
}
else {
    message("[qutils] App package name is not set.")
}

contains(QUTILS_FEATURES, multimedia) {
    message("[qutils] Multimedia is enabled.")
    QUTILS_NO_MULTIMEDIA=true
    !contains(QT, multimedia) {
        message("[qutils] Multimedia is not added to QT. Adding it to QT.")
        QT += multimedia
    }
}
else {
    message("[qutils] Multimedia is disabled.")
    QUTILS_NO_MULTIMEDIA=false
}

contains(QUTILS_FEATURES, for_mobile) {
    message("[qutils] Enabling mobile related features.")
    DEFINES += QUTILS_FOR_MOBILE=1
}
else {
    message("[qutils] Disabling mobile related features.")
    DEFINES += QUTILS_FOR_MOBILE=0
}

contains(QT, positioning) {
    message("[qutils] Enabling location related services.")
    DEFINES += QUTILS_LOCATION_ENABLED=1
}
else {
    message("[qutils] Location related services are NOT enabled.")
    DEFINES += QUTILS_LOCATION_ENABLED=0
}

FCM_ENABLED=false

contains(QUTILS_FEATURES, $$QUTILS_FEATURE_FCM) {
    message("[qutils] Firebase Cloud Messageing is enabled. It will only work on mobile devices.")
    FCM_ENABLED=true
    DEFINES += FCM_ENABLED=1
}
else {
    message("[qutils] Firebase Cloud Messageing is NOT enabled.")
    DEFINES += FCM_ENABLED=0
}

android {
    QT += androidextras

    isEqual(FCM_ENABLED, true) {
        OTHER_FILES += \
            $$PWD/android/src/org/zmc/qutils/notification/QutilsFirebaseInstanceIDService.java \
            $$PWD/android/src/org/zmc/qutils/notification/QutilsFirebaseMessagingService.java
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
        $$PWD/include/qutils/android/AndroidUtils.h \
        $$PWD/include/qutils/android/JNICallbacks.h

    SOURCES += \
        $$PWD/src/android/AndroidUtils.cpp \
        $$PWD/src/android/JNICallbacks.cpp

    INCLUDEPATH += $$PWD/include/android
}

ios {
    contains(QUTILS_FEATURES, no_photos) {
        DEFINES += QUTILS_PHOTOS_ENABLED=0
    }
    else {
        message("[qutils] Photos framework is enabled by default. To explicitly disable, add `no_photos` to QUTILS_FEATURES.")
        DEFINES += QUTILS_PHOTOS_ENABLED=1
        LIBS += -framework Photos
    }

    contains(QUTILS_FEATURES, $$QUTILS_FEATURE_SAFARI_SERVICES) {
        message("[qutils] SafariServices is enabled.")
        DEFINES += SAFARI_SERVICES_ENABLED
        LIBS += -framework SafariServices
    }

    isEqual(FCM_ENABLED, true) {
        LIBS += -framework UserNotifications
        QMAKE_LFLAGS += -ObjC

        MY_ENTITLEMENTS.name = CODE_SIGN_ENTITLEMENTS
        MY_ENTITLEMENTS.value = $$PWD/ios/pushnotifications.entitlements
        QMAKE_MAC_XCODE_SETTINGS += MY_ENTITLEMENTS
    }

    OBJECTIVE_HEADERS += \
        $$PWD/include/qutils/ios/iOSNativeUtils.h \
        $$PWD/include/qutils/ios/QutilsViewDelegate.h

    OBJECTIVE_SOURCES += \
        $$PWD/src/ios/iOSNativeUtils.mm \
        $$PWD/src/ios/AppDelegate.mm \
        $$PWD/src/ios/QutilsViewDelegate.mm

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
    $$PWD/src/NotificationClient.cpp \
    $$PWD/src/Notification.cpp \
    $$PWD/src/QutilsLog.cpp \
    $$PWD/src/FileUtils.cpp

equals(QUTILS_NO_MULTIMEDIA, true) {
    HEADERS += \
        $$PWD/include/qutils/AudioRecorder.h

    SOURCES += \
        $$PWD/src/AudioRecorder.cpp
}

INCLUDEPATH += $$PWD/include

DISTFILES += \
    $$PWD/android/src/org/zmc/qutils/Constants.java \
    $$PWD/src/ios/UIIMagePickerDelegate
