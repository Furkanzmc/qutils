CONFIG += c++11

android {
    QT += androidextras
    OTHER_FILES += \
        $$ANDROID_PACKAGE_SOURCE_DIR/src/org/zmc/qutils/notification/NotificationClient.java \
        $$ANDROID_PACKAGE_SOURCE_DIR/src/org/zmc/qutils/notification/NotificationReceiver.java \
        $$ANDROID_PACKAGE_SOURCE_DIR/src/org/zmc/qutils/notification/CppCallbacks.java \
        $$ANDROID_PACKAGE_SOURCE_DIR/src/org/zmc/qutils/QutilsActivity.java

    HEADERS += \
        $$PWD/include/zmc/Notification_Android.h \
        $$PWD/include/zmc/NotificationClient_Android.h

    SOURCES += \
        $$PWD/src/Notification_Android.cpp \
        $$PWD/src/NotificationClient_Android.cpp \
}

HEADERS += \
    $$PWD/include/zmc/ScreenHelper.h \
    $$PWD/include/zmc/QMLRefresh.h \
    $$PWD/include/zmc/NotificationClient.h \
    $$PWD/include/zmc/Notification.h \
    $$PWD/include/zmc/Macros.h

SOURCES += \
    $$PWD/src/ScreenHelper.cpp

INCLUDEPATH += $$PWD/include
