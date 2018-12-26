#include "qutils/QutilsPlugin.h"
// Qt
#include <QCoreApplication>
#include <QtQml/qqml.h>
// Local
#include "qutils/Macros.h"
#include "qutils/CppTypes.h"
#include "qutils/QutilsLog.h"
#include "qutils/FileUtils.h"
#include "qutils/SystemInfo.h"
#include "qutils/QMLRefresh.h"
#include "qutils/NativeUtils.h"
#include "qutils/DateManager.h"
#include "qutils/ScreenHelper.h"
#include "qutils/CacheManager.h"
#include "qutils/SignalManager.h"
#include "qutils/UpdateChecker.h"
#include "qutils/SettingsManager.h"
#include "qutils/Network/Download.h"
#include "qutils/PermissionManager.h"
#include "qutils/TranslationHelper.h"
#include "qutils/NotificationClient.h"
#include "qutils/Network/NetworkManager.h"
#include "qutils/Network/DownloadManager.h"
#if QUTILS_MULTIMEDIA_ENABLED
    #include "qutils/AudioRecorder.h"
#endif // QUTILS_MULTIMEDIA_ENABLED

QutilsPlugin::QutilsPlugin(QObject *parent)
    : QQmlExtensionPlugin(parent)
{

}

void QutilsPlugin::registerQutils(const char *uri)
{
    Q_ASSERT(QString(uri) == QString("qutils"));

#if QUTILS_MULTIMEDIA_ENABLED
    qmlRegisterType<zmc::AudioRecorder>("qutils", QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "AudioRecorder");
#endif // QUTILS_MULTIMEDIA_ENABLED

    qmlRegisterType<zmc::CacheManager>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "CacheManager");
    qmlRegisterUncreatableType<zmc::CppTypes>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "CppTypes", "Error: only enums");
    qmlRegisterType<zmc::DateManager>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "DateManager");

    qmlRegisterType<zmc::FileUtils>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "FileUtils");
    qmlRegisterType<zmc::NativeUtils>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "NativeUtils");
    qmlRegisterType<zmc::NotificationClient>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "NotificationClient");

    qmlRegisterType<zmc::QutilsLog>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "QutilsLog");
    qmlRegisterType<zmc::SettingsManager>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "SettingsManager");
    qmlRegisterType<zmc::SignalManager>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "SignalManager");

    qmlRegisterType<zmc::TranslationHelper>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "TranslationHelper");
    qmlRegisterType<zmc::UpdateChecker>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "UpdateChecker");
    qmlRegisterType<zmc::Network::DownloadManager>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "DownloadManager");

    qmlRegisterType<zmc::Network::Download>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "Download");
    qmlRegisterType<zmc::Network::NetworkManager>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "NetworkManager");
    qmlRegisterType<zmc::PermissionManager>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "PermissionManager");

    qmlRegisterSingletonType<zmc::SystemInfo>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "SystemInfo", zmc::SystemInfo::singletonProvider);
    qmlRegisterSingletonType<zmc::ScreenHelper>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "ScreenHelper", zmc::ScreenHelper::singletonProvider);
    qmlRegisterSingletonType<zmc::QMLRefresh>(uri, QUTILS_VER_MAJOR, QUTILS_VER_MINOR, "QMLRefresh", zmc::QMLRefresh::singletonProvider);
}

void QutilsPlugin::registerTypes(const char *uri)
{
    registerQutils(uri);
}

void QutilsPlugin::setPlatformContextProperties(QQmlContext *context)
{
#ifdef Q_OS_MACOS
    context->setContextProperty("Q_OS_MACOS", QVariant(true));
#else
    context->setContextProperty("Q_OS_MACOS", QVariant(false));
#endif // Q_OS_MACOS

#if defined(Q_OS_WIN) && !defined(Q_OS_WINRT)
    context->setContextProperty("Q_OS_WIN", QVariant(true));
#else
    context->setContextProperty("Q_OS_WIN", QVariant(false));
#endif // Q_OS_WIN

#if defined(Q_OS_WINRT)
    context->setContextProperty("Q_OS_WINRT", QVariant(true));
#else
    context->setContextProperty("Q_OS_WINRT", QVariant(false));
#endif // Q_OS_WINRT

#if defined(Q_OS_ANDROID)
    context->setContextProperty("Q_OS_ANDROID", QVariant(true));
#else
    context->setContextProperty("Q_OS_ANDROID", QVariant(false));
#endif // Q_OS_ANDROID

#if defined(Q_OS_IOS)
    context->setContextProperty("Q_OS_IOS", QVariant(true));
#else
    context->setContextProperty("Q_OS_IOS", QVariant(false));
#endif // Q_OS_IOS

#if defined(Q_OS_MOBILE)
    context->setContextProperty("Q_OS_MOBILE", QVariant(true));
#else
    context->setContextProperty("Q_OS_MOBILE", QVariant(false));
#endif // Q_OS_MOBILE

#if defined(Q_OS_DESKTOP)
    context->setContextProperty("Q_OS_DESKTOP", QVariant(true));
#else
    context->setContextProperty("Q_OS_DESKTOP", QVariant(false));
#endif // Q_OS_DESKTOP

    context->setContextProperty("FILE_PATH_PREFIX", QVariant(FILE_PATH_PREFIX));
}

#if QUTILS_DISABLE_AUTO_REGISTER == 0

static void startupFunctionQutils()
{
    QutilsPlugin::registerQutils();
}

Q_COREAPP_STARTUP_FUNCTION(startupFunctionQutils)

#endif // QUTILS_DISABLE_AUTO_REGISTER
