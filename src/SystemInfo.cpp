#include "qutils/SystemInfo.h"
#if defined(Q_OS_MAC)
    #include <sys/sysctl.h>
#endif // Q_OS_MAC
// Qt
#include <QSysInfo>
#include <QQmlEngine>
#include <QJSEngine>
//qutils
#include "qutils/Macros.h"
#if defined(Q_OS_ANDROID)
    #include "qutils/android/AndroidUtils.h"
#elif defined(Q_OS_IOS)
    #include "qutils/ios/iOSUtils.h"
#endif // Platform Check

namespace zmc
{

SystemInfo *SystemInfo::m_Instance = nullptr;

SystemInfo::SystemInfo(QObject *parent)
    : QObject(parent)
{

}

QObject *SystemInfo::singletonProvider(QQmlEngine *qmlEngine, QJSEngine *)
{
    if (m_Instance == nullptr) {
        m_Instance = new SystemInfo(qmlEngine);
    }

    return m_Instance;
}

QByteArray SystemInfo::bootUniqueId() const
{
    QByteArray result;

#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 11
    result = QSysInfo::bootUniqueId();
#endif // Qt Version Check

    return result;
}

QString SystemInfo::buildAbi() const
{
    return QSysInfo::buildAbi();
}

QString SystemInfo::buildCpuArchitecture() const
{
    return QSysInfo::buildCpuArchitecture();
}

QString SystemInfo::currentCpuArchitecture() const
{
    return QSysInfo::currentCpuArchitecture();
}

QString SystemInfo::kernelType() const
{
    return QSysInfo::kernelType();
}

QString SystemInfo::kernelVersion() const
{
    return QSysInfo::kernelVersion();
}

QString SystemInfo::machineHostName() const
{
    return QSysInfo::machineHostName();
}

QByteArray SystemInfo::machineUniqueId() const
{
    QByteArray result;

#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 11
    result = QSysInfo::machineUniqueId();
#endif // Qt Version Check

    return result;
}

QString SystemInfo::prettyProductName() const
{
    return QSysInfo::prettyProductName();
}

QString SystemInfo::productType() const
{
    return QSysInfo::productType();
}

QString SystemInfo::productVersion() const
{
    return QSysInfo::productVersion();
}

QString SystemInfo::deviceModel() const
{
    QString model = "";

#if defined(Q_OS_MACOS)
    size_t size;
    sysctlbyname("hw.model", NULL, &size, NULL, 0);
    char *name = new char[size];
    sysctlbyname("hw.model", name, &size, NULL, 0);
    model = QString::fromLatin1(name, size);
    free(name);
#elif defined(Q_OS_WIN)
    model = "windows";
#elif defined(Q_OS_WINRT)
    model = "winrt";
#elif defined(Q_OS_ANDROID)
    AndroidUtils utils;
    model = utils.getDeviceModel();
#elif defined(Q_OS_IOS)
    iOSUtils utils;
    model = utils.getDeviceName();
#endif // Platform Check

    return model;
}

}
