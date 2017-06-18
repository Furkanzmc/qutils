#include "qutils/SignalManager.h"

namespace zmc
{

QVector<SignalManager *> SignalManager::m_Instances = QVector<SignalManager *>();
unsigned int SignalManager::m_LastInstanceIndex = 0;

SignalManager::SignalManager(QObject *parent)
    : QObject(parent)
    , m_InstanceIndex(m_LastInstanceIndex)
{
    m_Instances.push_back(this);
}

SignalManager::~SignalManager()
{
    m_Instances[m_InstanceIndex] = nullptr;
}

void SignalManager::emitSignal(const QString &signalName)
{
    for (SignalManager *instance : m_Instances) {
        if (instance) {
            instance->emitSignalPrivate(signalName);
        }
    }
}

void SignalManager::emitSignalPrivate(const QString &signalName)
{
    emit signalReceived(signalName);
}

}
