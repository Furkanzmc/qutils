#include "qutils/SignalManager.h"

namespace zmc
{

QMap<int, SignalManager *> SignalManager::m_Instances = QMap<int, SignalManager *>();
unsigned int SignalManager::m_NextInstanceID = 0;

SignalManager::SignalManager(QObject *parent)
    : QObject(parent)
    , m_InstanceIndex(m_NextInstanceID)
{
    m_NextInstanceID++;
    m_Instances.insert(m_InstanceIndex, this);
}

SignalManager::~SignalManager()
{
    m_Instances.remove(m_InstanceIndex);
}

void SignalManager::emitSignal(const QString &signalName, const QString &targetObjectName, const QVariantMap &data)
{
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        SignalManager *instance = it.value();

        if (instance) {
            if (targetObjectName.isEmpty()) {
                QMetaObject::invokeMethod(instance, std::bind(&SignalManager::signalReceived, instance, signalName, data), Qt::QueuedConnection);
            }
            else if (targetObjectName == instance->objectName()) {
                QMetaObject::invokeMethod(instance, std::bind(&SignalManager::signalReceived, instance, signalName, data), Qt::QueuedConnection);
                break;
            }
        }
    }
}

}
