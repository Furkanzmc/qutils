#include "qutils/macos/MacOSUtils.h"
// Qt
#include <QGuiApplication>
#include <QFileOpenEvent>
#include <QTimer>
// Local
#include "qutils/Macros.h"

namespace zmc
{

QList<MacOSUtils *> MacOSUtils::m_Instances = QList<MacOSUtils *>();

MacOSUtils::MacOSUtils(QObject *parent)
    : QObject(parent)
    , m_InstanceID(m_Instances.size())
    , m_IsMainController(false)
{
    m_Instances.push_back(this);
}

MacOSUtils::~MacOSUtils()
{
    m_Instances[m_InstanceID] = nullptr;
}

bool MacOSUtils::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent *fileEvent = static_cast<QFileOpenEvent *>(event);
        const QUrl url = fileEvent->url();
        if (url.isValid()) {
            // FIXME: The resulting URL is always empty.
            emitOpenedWithURLSignal(fileEvent->url().toString());
        }

        return true;
    }

    return QObject::eventFilter(obj, event);
}

bool MacOSUtils::isMainController() const
{
    return m_IsMainController;
}

void MacOSUtils::setMainController(bool isMain, bool disableOthers)
{
    if (disableOthers) {
        const int currentCount = m_Instances.count();
        for (int index = 0; index < currentCount; index++) {
            MacOSUtils *utils = m_Instances.at(index);
            if (utils && utils->isMainController()) {
                utils->setMainController(false, false);
                break;
            }
        }
    }

    if (m_IsMainController != isMain) {
        if (isMain) {
            qApp->installEventFilter(this);
        }
        else {
            qApp->removeEventFilter(this);
        }

        emit mainControllerChanged();
        m_IsMainController = isMain;
    }
}

void MacOSUtils::emitOpenedWithURLSignal(QString url)
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        MacOSUtils *utils = m_Instances.at(index);
        if (utils && utils->isMainController()) {
            emit utils->openedWithURL(url);
            break;
        }
    }
}

void MacOSUtils::emitOpenedWithoutURLSignal()
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        MacOSUtils *utils = m_Instances.at(index);
        if (utils && utils->isMainController()) {
            emit utils->openedWithoutURL();
            break;
        }
    }
}

}
