#include "qutils/QMLRefresh.h"
// Qt
#include <QTimer>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
// Local
#include "qutils/Macros.h"

namespace zmc
{

QMLRefresh *QMLRefresh::m_Instance = nullptr;

QMLRefresh::QMLRefresh(QObject *parent)
    : QObject(parent)
    , m_Engine(nullptr)
    , m_Window(nullptr)
{

}

QObject *QMLRefresh::singletonProvider(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    if (m_Instance == nullptr) {
        m_Instance = new QMLRefresh(qmlEngine);
    }

    m_Instance->m_Engine = dynamic_cast<QQmlApplicationEngine *>(qmlEngine);
    Q_UNUSED(jsEngine);
    return m_Instance;
}

void QMLRefresh::reload(float delay)
{
    if (m_Engine == nullptr) {
        LOG_ERROR("QML Engine is not yet loaded.");
        return;
    }

    const QObjectList objects = m_Engine->rootObjects();
    if (!objects.isEmpty()) {
        m_Window = static_cast<QQuickWindow *>(objects.at(0));
    }

    if (m_Window == nullptr) {
        LOG_WARNING("QML Window is not loaded.");
    }

    if (delay == 0.f) {
        reloadCache();
    }
    else {
        QTimer::singleShot(delay * 1000, this, &QMLRefresh::reloadCache);
    }
}

QString QMLRefresh::getMainQMLFile() const
{
    return m_MainQMLFile;
}

void QMLRefresh::setMainQMLFile(const QString &file)
{
    m_MainQMLFile = file;
}

void QMLRefresh::reloadCache()
{
    if (m_MainQMLFile.length() == 0) {
        LOG_ERROR("m_MainQMLFile is empty.");
        return;
    }

    if (m_Window) {
        m_Window->close();
    }

    m_Engine->clearComponentCache();
    m_Engine->load(QUrl(m_MainQMLFile));
}

}
