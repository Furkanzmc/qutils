CONFIG += c++11

android {
    QT += androidextras
    OTHER_FILES += \
        $$PWD/android/src/org/zmc/qutils/notification/NotificationClient.java \
        $$PWD/android/src/org/zmc/qutils/notification/NotificationReceiver.java \
        $$PWD/android/src/org/zmc/qutils/notification/CppCallbacks.java \
        $$PWD/android/src/org/zmc/qutils/QutilsActivity.java \
        $$PWD/android/src/org/zmc/qutils/AndroidUtils.java

    HEADERS += \
        $$PWD/include/zmc/android/Notification_Android.h \
        $$PWD/include/zmc/android/NotificationClient_Android.h \
        $$PWD/include/zmc/android/AndroidUtils.h

    SOURCES += \
        $$PWD/src/android/Notification_Android.cpp \
        $$PWD/src/android/NotificationClient_Android.cpp \
        $$PWD/src/android/AndroidUtils.cpp

    INCLUDEPATH += $$PWD/include/android
}

HEADERS += \
    $$PWD/include/zmc/ScreenHelper.h \
    $$PWD/include/zmc/QMLRefresh.h \
    $$PWD/include/zmc/NotificationClient.h \
    $$PWD/include/zmc/Notification.h \
    $$PWD/include/zmc/Macros.h \

SOURCES += \
    $$PWD/src/ScreenHelper.cpp


INCLUDEPATH += $$PWD/include
