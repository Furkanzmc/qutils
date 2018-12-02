#pragma once
// std
#include <iostream>
// Qt
#include <QDebug>
#include <qsystemdetection.h>
#ifdef Q_OS_ANDROID
    #include <android/log.h>
#endif // Q_OS_ANDROID

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#if defined(Q_OS_WIN) || defined(Q_OS_WINRT)
    #define FILE_PATH_PREFIX "file:///"
#else
    #define FILE_PATH_PREFIX "file://"
#endif // defined(Q_OS_WIN)

#if (!defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)) && (defined(Q_OS_WIN) || defined(Q_OS_MACOS) || defined(Q_OS_WINRT))
    #define Q_OS_DESKTOP
#endif // Desktop Platform

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    #define Q_OS_MOBILE
#endif // Mobile Platform

#if QUTILS_DISABLE_LOGS
    #define LOG(msg)
    #define LOG_WARNING(msg)
    #define LOG_ERROR(msg)
    #define LOG_STD(msg)
    #define LOG_WARNING_STD(msg)
    #define LOG_ERROR_STD(msg)
    #define LOG_JNI(msg)
    #define LOG_WARNING_JNI(msg)
    #define LOG_ERROR_JNI(msg)
#else
    #if defined(Q_OS_MOBILE)
        #if defined(__clang__)
            #define LOG(msg) qDebug() << "[INFO]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg
            #define LOG_WARNING(msg) qDebug() << "[WARNING]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg
            #define LOG_ERROR(msg) qDebug() << "[ERROR]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg
            #define LOG_STD(msg) std::clog << "[INFO]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg << "\n"
            #define LOG_WARNING_STD(msg) std::clog << "[WARNING]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg <<  "\n"
            #define LOG_ERROR_STD(msg) std::cerr << "[ERROR]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg << "\n"
        #else // __clang__
            #define LOG(msg) qDebug() << "[INFO] " << msg
            #define LOG_WARNING(msg) qDebug() << "[WARNING] " << msg
            #define LOG_ERROR(msg) qDebug() << "[ERROR] " << msg
            #define LOG_STD(msg) std::clog << "[INFO] " << msg << "\n"
            #define LOG_WARNING_STD(msg) std::clog << "[WARNING] " << msg << "\n"
            #define LOG_ERROR_STD(msg) std::cerr << "[ERROR] " << msg << std::endl
        #endif // __clang__
    #else
        #define LOG(msg) qDebug() << "[INFO]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg
        #define LOG_WARNING(msg) qDebug() << "[WARNING]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg
        #define LOG_ERROR(msg) qDebug() << "[ERROR]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg
        #define LOG_STD(msg) std::clog << "[INFO] " << __FUNCTION__ << "(" << __LINE__ << "):" << msg << "\n"
        #define LOG_WARNING_STD(msg) std::clog << "[WARNING] " << __FUNCTION__ << "(" << __LINE__ << "):" << msg << "\n"
        #define LOG_ERROR_STD(msg) std::cerr << "[ERROR] " << __FUNCTION__ << "(" << __LINE__ << "):" << msg << std::endl
    #endif // Q_OS_MOBILE

    #ifdef Q_OS_ANDROID
        #ifdef QUTILS_APP_PACKAGE_NAME
            #define APP_TAG STRINGIFY(QUTILS_APP_PACKAGE_NAME)
        #else
            #define APP_TAG "com.zmc.qutils"
        #endif // QUTILS_APP_PACKAGE_NAME
        #define LOG_JNI(msg) __android_log_write(ANDROID_LOG_INFO, APP_TAG, msg);
        #define LOG_WARNING_JNI(msg) __android_log_write(ANDROID_LOG_WARN, APP_TAG, msg)
        #define LOG_ERROR_JNI(msg) __android_log_write(ANDROID_LOG_ERROR, APP_TAG, msg)
    #else
        #define LOG_JNI(msg) Q_UNUSED(msg)
        #define LOG_WARNING_JNI(msg) Q_UNUSED(msg)
        #define LOG_ERROR_JNI(msg) Q_UNUSED(msg)
    #endif // Q_OS_ANDROID
#endif // QUTILS_DISABLE_LOGS
