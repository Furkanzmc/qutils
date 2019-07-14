VER_MAJOR = 1
VER_MINOR = 0

DEFINES += QUTILS_VER_MAJOR=1
DEFINES += QUTILS_VER_MINOR=0

CONFIG += c++11
QT += sql network

# Available QUTILS_FEATURES options:
#    - no_photos: Disables the Photos framework on iOS.
#    - multimedia: Enables multimedia features.
#    - for_mobile: Enables mobile related features.
#    - disable_logs: Disables the LOG macros.
#    - disable_auto_register: Add this to disable auto registering QML types.
#    - camera_enabled: [iOS Only] This is used to enable camera related functions such as permissions. It links AVFoundation.
#    - safari_services: Add in-app browser support.

# Available Options:
#    - QUTILS_APP_NAME: Used for naming auto-generated files.
#    - QUTILS_CACHE_DB_FILE_NAME: Used for naming cache file.
#    - QUTILS_SETTINGS_DB_FILE_NAME: Used for naming settings file.
#    - QUTILS_APP_PACKAGE_NAME: This is the package name of the app.

QUTILS_FEATURE_SAFARI_SERVICES = safari_services

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

!isEmpty(QUTILS_CACHE_DB_FILE_NAME) {
    message("[qutils] App cache file name is set to" $$QUTILS_CACHE_DB_FILE_NAME)
    DEFINES += QUTILS_CACHE_DB_FILE_NAME=$$QUTILS_CACHE_DB_FILE_NAME
}

!isEmpty(QUTILS_SETTINGS_DB_FILE_NAME) {
    message("[qutils] App settings file name is set to" $$QUTILS_SETTINGS_DB_FILE_NAME)
    DEFINES += QUTILS_SETTINGS_DB_FILE_NAME=$$QUTILS_SETTINGS_DB_FILE_NAME
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

contains(QUTILS_FEATURES, disable_auto_register) {
    message("[qutils] Disabling auto register for QML types.")
    DEFINES += QUTILS_DISABLE_AUTO_REGISTER=1
}
else {
    message("[qutils] Enabling auto register for QML types.")
    DEFINES += QUTILS_DISABLE_AUTO_REGISTER=0
}

contains(QUTILS_FEATURES, for_mobile) {
    message("[qutils] Enabling mobile related features.")
    DEFINES += QUTILS_FOR_MOBILE=1
}
else {
    message("[qutils] Disabling mobile related features.")
    DEFINES += QUTILS_FOR_MOBILE=0
}

contains(QUTILS_FEATURES, disable_logs) {
    message("[qutils] Disabling logs.")
    DEFINES += QUTILS_DISABLE_LOGS=1
}
else {
    DEFINES += QUTILS_DISABLE_LOGS=0
}

contains(QT, positioning) {
    message("[qutils] Enabling location related services.")
    DEFINES += QUTILS_LOCATION_ENABLED=1
}
else {
    message("[qutils] Location related services are NOT enabled.")
    DEFINES += QUTILS_LOCATION_ENABLED=0
}

android {
    QT += androidextras

    OTHER_FILES += \
        $$PWD/android/src/org/zmc/qutils/CppCallbacks.java \
        $$PWD/android/src/org/zmc/qutils/QutilsActivity.java \
        $$PWD/android/src/org/zmc/qutils/AndroidUtils.java

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

    contains(QUTILS_FEATURES, camera_enabled) {
        DEFINES += QUTILS_CAMERA_ENABLED=1
        LIBS += -framework AVFoundation
        message("[qutils] Camera related functions are enabled.")
    }
    else {
        DEFINES += QUTILS_CAMERA_ENABLED=0
    }

    contains(QUTILS_FEATURES, $$QUTILS_FEATURE_SAFARI_SERVICES) {
        message("[qutils] SafariServices is enabled.")
        DEFINES += SAFARI_SERVICES_ENABLED
        LIBS += -framework SafariServices
    }

    contains(QTFIREBASE_CONFIG, messaging) {
        MY_ENTITLEMENTS.name = CODE_SIGN_ENTITLEMENTS
        MY_ENTITLEMENTS.value = $$PWD/ios/pushnotifications.entitlements
        QMAKE_MAC_XCODE_SETTINGS += MY_ENTITLEMENTS
    }

    OBJECTIVE_HEADERS += \
        $$PWD/include/qutils/ios/iOSNativeUtils.h \
        $$PWD/include/qutils/ios/QutilsViewDelegate.h \
        $$PWD/include/qutils/ios/DocumentPicker.h \
        $$PWD/include/qutils/ios/FileUtils_Private.h \
        $$PWD/include/qutils/ios/PermissionManagerPrivate.h

    OBJECTIVE_SOURCES += \
        $$PWD/src/ios/iOSNativeUtils.mm \
        $$PWD/src/ios/AppDelegate.mm \
        $$PWD/src/ios/QutilsViewDelegate.mm \
        $$PWD/src/ios/FileUtils_Private.mm \
        $$PWD/src/ios/DocumentPicker.mm \
        $$PWD/src/ios/PermissionManagerPrivate.mm

    HEADERS += \
        $$PWD/include/qutils/ios/iOSUtils.h

    SOURCES += \
        $$PWD/src/ios/iOSUtils.cpp
}

osx {
    HEADERS += $$PWD/include/qutils/macos/MacOSUtils.h
    SOURCES += $$PWD/src/macos/MacOSUtils.cpp
}

HEADERS += \
    $$PWD/include/qutils/ScreenHelper.h \
    $$PWD/include/qutils/NotificationClient.h \
    $$PWD/include/qutils/Notification.h \
    $$PWD/include/qutils/Macros.h \
    $$PWD/include/qutils/TranslationHelper.h \
    $$PWD/include/qutils/NativeUtils.h \
    $$PWD/include/qutils/SqliteManager.h \
    $$PWD/include/qutils/SettingsManager.h \
    $$PWD/include/qutils/CacheManager.h \
    $$PWD/include/qutils/Network/NetworkManager.h \
    $$PWD/include/qutils/Network/Download.h \
    $$PWD/include/qutils/Network/DownloadManager.h \
    $$PWD/include/qutils/JsonUtils.h \
    $$PWD/include/qutils/SignalManager.h \
    $$PWD/include/qutils/DateManager.h \
    $$PWD/include/qutils/QutilsLog.h \
    $$PWD/include/qutils/FileUtils.h \
    $$PWD/include/qutils/CppTypes.h \
    $$PWD/include/qutils/UpdateChecker.h \
    $$PWD/include/qutils/SystemInfo.h \
    $$PWD/include/qutils/PermissionManager.h

SOURCES += \
    $$PWD/src/ScreenHelper.cpp \
    $$PWD/src/TranslationHelper.cpp \
    $$PWD/src/NativeUtils.cpp \
    $$PWD/src/SqliteManager.cpp \
    $$PWD/src/SettingsManager.cpp \
    $$PWD/src/CacheManager.cpp \
    $$PWD/src/Network/NetworkManager.cpp \
    $$PWD/src/Network/Download.cpp \
    $$PWD/src/Network/DownloadManager.cpp \
    $$PWD/src/JsonUtils.cpp \
    $$PWD/src/SignalManager.cpp \
    $$PWD/src/DateManager.cpp \
    $$PWD/src/NotificationClient.cpp \
    $$PWD/src/Notification.cpp \
    $$PWD/src/QutilsLog.cpp \
    $$PWD/src/FileUtils.cpp \
    $$PWD/src/UpdateChecker.cpp \
    $$PWD/src/SystemInfo.cpp \
    $$PWD/src/PermissionManager.cpp

contains(QT, qml) {
    HEADERS += \
        $$PWD/include/qutils/QMLRefresh.h \
        $$PWD/include/qutils/QutilsPlugin.h

    SOURCES += \
        $$PWD/src/QMLRefresh.cpp \
        $$PWD/src/QutilsPlugin.cpp
}

equals(QUTILS_NO_MULTIMEDIA, true) {
    HEADERS += \
        $$PWD/include/qutils/AudioRecorder.h

    SOURCES += \
        $$PWD/src/AudioRecorder.cpp

    DEFINES +=QUTILS_MULTIMEDIA_ENABLED=1
}
else {
    DEFINES +=QUTILS_MULTIMEDIA_ENABLED=0
}

INCLUDEPATH += $$PWD/include

DISTFILES += \
    $$PWD/android/src/org/zmc/qutils/Constants.java \
    $$PWD/src/ios/UIIMagePickerDelegate
