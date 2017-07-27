#include "qutils/SignalManager.h"

namespace zmc
{

QVector<SignalManager *> SignalManager::m_Instances = QVector<SignalManager *>();

SignalManager::SignalManager(QObject *parent)
    : QObject(parent)
    , m_InstanceIndex(m_Instances.size())
{
    m_Instances.push_back(this);
}

SignalManager::~SignalManager()
{
    m_Instances[m_InstanceIndex] = nullptr;
}

void SignalManager::emitSignal(const QString &signalName, const QString &targetObjectName)
{
    for (SignalManager *instance : m_Instances) {
        if (instance) {
            if (targetObjectName.length() > 0) {
                if (targetObjectName == instance->objectName()) {
                    instance->emitSignalPrivate(signalName);
                }
            }
            else {
                instance->emitSignalPrivate(signalName);
            }
        }
    }
}

void SignalManager::emitSignalPrivate(const QString &signalName)
{
    emit signalReceived(signalName);
}

}
