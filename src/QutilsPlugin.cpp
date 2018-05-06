#include "qutils/QutilsPlugin.h"
// Qt
#include <QCoreApplication>
#include <QtQml/qqml.h>
// Local
#include "qutils/NotificationClient.h"
#include "qutils/TranslationHelper.h"
#include "qutils/SettingsManager.h"
#include "qutils/SignalManager.h"
#include "qutils/CacheManager.h"
#include "qutils/ScreenHelper.h"
#include "qutils/NativeUtils.h"
#include "qutils/DateManager.h"
#include "qutils/FileUtils.h"
#include "qutils/QutilsLog.h"
#include "qutils/CppTypes.h"
#include "qutils/Macros.h"
#if QUTILS_MULTIMEDIA_ENABLED
#  include "qutils/AudioRecorder.h"
#endif // QUTILS_MULTIMEDIA_ENABLED
#ifdef Q_OS_DESKTOP
#  include "qutils/QMLRefresh.h"
#endif // Q_OS_DESKTOP

QutilsPlugin::QutilsPlugin(QObject *parent)
    : QQmlExtensionPlugin(parent)
{

}

void QutilsPlugin::registerQutils(const char *uri)
{
    Q_ASSERT(QString(uri) == QString("qutils"));

#if QUTILS_MULTIMEDIA_ENABLED
    qmlRegisterType<zmc::AudioRecorder>("qutils", 1, 0, "AudioRecorder");
#endif // QUTILS_MULTIMEDIA_ENABLED

    qmlRegisterType<zmc::CacheManager>("qutils", 1, 0, "CacheManager");
    qmlRegisterUncreatableType<zmc::CppTypes>("qutils", 1, 0, "CppTypes", "Error: only enums");
    qmlRegisterType<zmc::DateManager>("qutils", 1, 0, "DateManager");

    qmlRegisterType<zmc::FileUtils>("qutils", 1, 0, "FileUtils");
    qmlRegisterType<zmc::NativeUtils>("qutils", 1, 0, "NativeUtils");
    qmlRegisterType<zmc::NotificationClient>("qutils", 1, 0, "NotificationClient");

    qmlRegisterType<zmc::QutilsLog>("qutils", 1, 0, "QutilsLog");
    qmlRegisterType<zmc::ScreenHelper>("qutils", 1, 0, "ScreenHelper");
    qmlRegisterType<zmc::SettingsManager>("qutils", 1, 0, "SettingsManager");

    qmlRegisterType<zmc::SignalManager>("qutils", 1, 0, "SignalManager");
    qmlRegisterType<zmc::TranslationHelper>("qutils", 1, 0, "TranslationHelper");
}

void QutilsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString(uri) == QString("qutils"));

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

#if !defined(Q_OS_WIN) && defined(Q_OS_WINRT)
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

static void startupFunctionQutils()
{
    QutilsPlugin::registerQutils();
}

Q_COREAPP_STARTUP_FUNCTION(startupFunctionQutils)
