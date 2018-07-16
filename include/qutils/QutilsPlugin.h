#pragma once
// Qt
#include <QQmlContext>
#include <QQmlExtensionPlugin>

class QutilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT

public:
    explicit QutilsPlugin(QObject *parent = nullptr);

    void registerTypes(const char *uri);

    static void registerQutils(const char *uri = "qutils");
    static void setPlatformContextProperties(QQmlContext *context);
};
