#pragma once
#include <QObject>
#include <QVector>

namespace zmc
{

class SignalManager : public QObject
{
    Q_OBJECT
public:
    explicit SignalManager(QObject *parent = nullptr);
    ~SignalManager();

    Q_INVOKABLE void emitSignal(const QString &signalName);

signals:
    void signalReceived(const QString &signalName);

private:
    static QVector<SignalManager *> m_Instances;
    static unsigned int m_LastInstanceIndex;

    const unsigned int m_InstanceIndex;

private:
    void emitSignalPrivate(const QString &signalName);
};

}
