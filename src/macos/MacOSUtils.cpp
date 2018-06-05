#include "qutils/macos/MacOSUtils.h"
// Qt
#include <QGuiApplication>
#include <QFileOpenEvent>
#include <QTimer>
// Local
#include "qutils/Macros.h"

namespace zmc
{

QMap<int, MacOSUtils *> MacOSUtils::m_Instances = QMap<int, MacOSUtils *>();

MacOSUtils::MacOSUtils(QObject *parent)
    : QObject(parent)
    , m_InstanceID(m_Instances.size())
    , m_IsMainController(false)
{
    m_Instances.insert(m_InstanceID, this);
}

MacOSUtils::~MacOSUtils()
{
    m_Instances.remove(m_InstanceID);
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
        auto begin = m_Instances.begin();
        auto end = m_Instances.end();
        for (auto it = begin; it != end; it++) {
            MacOSUtils *utils = it.value();
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
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        MacOSUtils *utils = it.value();
        if (utils && utils->isMainController()) {
            emit utils->openedWithURL(url);
            break;
        }
    }
}

void MacOSUtils::emitOpenedWithoutURLSignal()
{
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        MacOSUtils *utils = it.value();
        if (utils && utils->isMainController()) {
            emit utils->openedWithoutURL();
            break;
        }
    }
}

}
