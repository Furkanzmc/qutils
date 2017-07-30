#pragma once
#include <QObject>

namespace zmc
{

/**
 * @brief This class can be used to log to the logcat on Android devices from QML.
 */
class QutilsLog : public QObject
{
    Q_OBJECT

public:
    explicit QutilsLog(QObject *parent = nullptr);

    Q_INVOKABLE void info(const QString &msg);
    Q_INVOKABLE void warning(const QString &msg);
    Q_INVOKABLE void error(const QString &msg);
};

}
