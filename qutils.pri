CONFIG += c++11
QT += sql

android {
    QT += androidextras
    OTHER_FILES += \
        $$PWD/android/src/org/zmc/qutils/notification/NotificationClient.java \
        $$PWD/android/src/org/zmc/qutils/notification/NotificationReceiver.java \
        $$PWD/android/src/org/zmc/qutils/notification/CppCallbacks.java \
        $$PWD/android/src/org/zmc/qutils/QutilsActivity.java \
        $$PWD/android/src/org/zmc/qutils/AndroidUtils.java

    HEADERS += \
        $$PWD/include/qutils/android/Notification_Android.h \
        $$PWD/include/qutils/android/NotificationClient_Android.h \
        $$PWD/include/qutils/android/AndroidUtils.h

    SOURCES += \
        $$PWD/src/android/Notification_Android.cpp \
        $$PWD/src/android/NotificationClient_Android.cpp \
        $$PWD/src/android/AndroidUtils.cpp

    INCLUDEPATH += $$PWD/include/android
}

HEADERS += \
    $$PWD/include/qutils/ScreenHelper.h \
    $$PWD/include/qutils/QMLRefresh.h \
    $$PWD/include/qutils/NotificationClient.h \
    $$PWD/include/qutils/Notification.h \
    $$PWD/include/qutils/Macros.h \
    $$PWD/include/qutils/TranslationHelper.h \
    $$PWD/include/qutils/NativeUtils.h \
    $$PWD/include/qutils/SqliteManager.h

SOURCES += \
    $$PWD/src/ScreenHelper.cpp \
    $$PWD/src/TranslationHelper.cpp \
    $$PWD/src/NativeUtils.cpp \
    $$PWD/src/SqliteManager.cpp


INCLUDEPATH += $$PWD/include
