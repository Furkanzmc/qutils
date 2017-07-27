#include "qutils/QMLRefresh.h"
// Local
#include "qutils/Macros.h"

namespace zmc
{

QMLRefresh::QMLRefresh(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent)
    , m_Engine(engine)
    , m_Window(nullptr)
{

}

void QMLRefresh::reload(float delay)
{
    if (delay == 0.f) {
        reloadCache();
    }
    else {
        QTimer::singleShot(delay * 1000, this, &QMLRefresh::reloadCache);
    }
}

void QMLRefresh::setWindow(QQuickWindow *window)
{
    m_Window = window;
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
