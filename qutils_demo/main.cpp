#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#ifdef Q_OS_ANDROID
#include "qutils/NotificationClient.h"
#include "qutils/Notification.h"
#include "qutils/android/AndroidUtils.h"
#endif // Q_OS_ANDROID
#include "qutils/ScreenHelper.h"
#include "qutils/QMLRefresh.h"
#include "qutils/Macros.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    zmc::ScreenHelper screenHelper;
    engine.rootContext()->setContextProperty("SH", &screenHelper);

#ifdef Q_OS_ANDROID
    qmlRegisterType<zmc::NotificationClient>("NotificationClient", 1, 0, "NotificationClient");
    qmlRegisterType<zmc::Notification>("NotificationClient", 1, 0, "Notification");
    qmlRegisterType<zmc::AndroidUtils>("qutils.AndroidUtils", 1, 0, "AndroidUtils");
#endif // Q_OS_ANDROID

#if defined(QT_DEBUG) && defined(Q_OS_DESKTOP)
    zmc::QMLRefresh manager(&engine);
    manager.setMainQMLFile("file:///E:/Development/SourceTree/GitHub/qutils/qutils_demo/qml/main.qml");
    engine.rootContext()->setContextProperty("QM", &manager);
    engine.rootContext()->setContextProperty("QML_DEBUG", QVariant::fromValue<bool>(true));
    engine.rootContext()->setContextProperty("Q_OS_DESKTOP", QVariant::fromValue<int>(1));
    engine.load(QUrl("file:///E:/Development/SourceTree/GitHub/qutils/qutils_demo/qml/main.qml"));
#else
    engine.rootContext()->setContextProperty("QM", nullptr);
    engine.rootContext()->setContextProperty("QML_DEBUG", QVariant::fromValue<bool>(false));
    engine.rootContext()->setContextProperty("Q_OS_DESKTOP", QVariant::fromValue<int>(0));
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
#endif // QT_DEBUG


    return app.exec();
}
