#pragma once
#include <QObject>
#include <QVector>
#include <QVariant>

namespace zmc
{

/**
 * @brief The SignalManager class is used to send signals between different parts of the program. You can use this with your QML application to notify a change
 * from one file to another. The signals are sent to all of the instances unless a specific target is set.
 */
class SignalManager : public QObject
{
    Q_OBJECT
public:
    explicit SignalManager(QObject *parent = nullptr);
    ~SignalManager();

    /**
     * @brief Send a signal to all of the instances. If you set an target object name, only that target is notified of the signal.
     * @param signalName
     * @param targetObjectName
     */
    Q_INVOKABLE void emitSignal(const QString &signalName, const QString &targetObjectName = "", const QVariantMap data = QVariantMap());

signals:
    /**
     * @brief If there was no data passed to the signal, it will be null.
     * @param signalName
     * @param data
     */
    void signalReceived(const QString &signalName, QVariantMap data);

private:
    static QVector<SignalManager *> m_Instances;

    const unsigned int m_InstanceIndex;

private:
    void emitSignalPrivate(const QString &signalName, const QVariantMap data);
};

}
