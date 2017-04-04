#include "qutils/ScreenHelper.h"
// Qt
#include <QGuiApplication>
#include <QScreen>
// zmc
#include "qutils/Macros.h"

namespace zmc
{

ScreenHelper::ScreenHelper(QObject *parent)
    : QObject(parent)
    , m_DPI(QGuiApplication::primaryScreen()->physicalDotsPerInch())
{

}

qreal ScreenHelper::dp(const qreal &size)
{
#if defined(Q_OS_DESKTOP)
    return round(size * (m_DPI / 90.0));
#elif defined(Q_OS_ANDROID)
    return round(size * (m_DPI / 160.0));
#endif // Platform Check
}

}
