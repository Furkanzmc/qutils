#include "qutils/ScreenHelper.h"
// Qt
#include <QGuiApplication>
#include <QScreen>
#ifdef QT_DEBUG
#include <QDebug>
#endif // QT_DEBUG
// zmc
#include "qutils/Macros.h"

namespace zmc
{

ScreenHelper::ScreenHelper(QObject *parent)
    : QObject(parent)
    , m_DPI(QGuiApplication::primaryScreen()->physicalDotsPerInch())
    , m_LowDPIValue(120)
    , m_MediumDPIValue(160)
    , m_HighDPIValue(240)
    , m_XHighDPIValue(320)
    , m_XXHighDPIValue(480)
    , m_XXXHighDPIValue(640)
    , m_DPIVariation(20)
{
#ifdef QT_DEBUG
    qDebug() << "The DPI is: " << m_DPI;
#endif // QT_DEBUG
}

qreal ScreenHelper::dp(const qreal &size)
{
    qreal newSize = size;
#if defined(Q_OS_DESKTOP)
    if (m_DPI <= 90) {
        newSize = round(size * (m_DPI / 90.0));
    }
#elif defined(Q_OS_ANDROID)
    newSize = round(size * (m_DPI / 160.0));
#endif // Platform Check

    return newSize;
}

QString ScreenHelper::getLowResourceFolderName() const
{
    return m_LowResourceName;
}

void ScreenHelper::setLowResourceFolderName(const QString &resourceName)
{
    m_LowResourceName = resourceName;
}

QString ScreenHelper::getMediumResourceFolderName() const
{
    return m_MediumResourceName;
}

void ScreenHelper::setMediumResourceFolderName(const QString &resourceName)
{
    m_MediumResourceName = resourceName;
}

QString ScreenHelper::getHighResourceFolderName() const
{
    return m_HighResourceName;
}

void ScreenHelper::setHighResourceFolderName(const QString &resourceName)
{
    m_HighResourceName = resourceName;
}

QString ScreenHelper::getXHighResourceFolderName() const
{
    return m_XHighResourceName;
}

void ScreenHelper::setXHighResourceFolderName(const QString &resourceName)
{
    m_XHighResourceName = resourceName;
}

QString ScreenHelper::getXXHighResourceFolderName() const
{
    return m_XXHighResourceName;
}

void ScreenHelper::setXXHighResourceFolderName(const QString &resourceName)
{
    m_XXHighResourceName = resourceName;
}

QString ScreenHelper::getXXXHighResourceFolderName() const
{
    return m_XXXHighResourceName;
}

void ScreenHelper::setXXXHighResourceFolderName(const QString &resourceName)
{
    m_XXXHighResourceName = resourceName;
}

QString ScreenHelper::getResourceFolderName() const
{
    QString name = getLowResourceFolderName();

    if (isMDPI()) {
        name = getMediumResourceFolderName();
    }
    else if (isHDPI()) {
        name = getHighResourceFolderName();
    }
    else if (isXHDPI()) {
        name = getXHighResourceFolderName();
    }
    else if (isXXHDPI()) {
        name = getXXHighResourceFolderName();
    }
    else if (isXXXHDPI()) {
        name = getXXXHighResourceFolderName();
    }

    return name;
}

QString ScreenHelper::getResource(const QString &fileName) const
{
    return getResourceFolderName() + "/" + fileName;
}

bool ScreenHelper::isLDPI() const
{
    return m_DPI < m_LowDPIValue + m_DPIVariation;
}

bool ScreenHelper::isMDPI() const
{
    return m_DPI < m_MediumDPIValue + m_DPIVariation && m_DPI > m_LowDPIValue + m_DPIVariation;
}

bool ScreenHelper::isHDPI() const
{
    return m_DPI < m_HighDPIValue + m_DPIVariation && m_DPI > m_MediumDPIValue + m_DPIVariation;
}

bool ScreenHelper::isXHDPI() const
{
    return m_DPI < m_XHighDPIValue + m_DPIVariation && m_DPI > m_HighDPIValue + m_DPIVariation;
}

bool ScreenHelper::isXXHDPI() const
{
    return m_DPI < m_XXHighDPIValue + m_DPIVariation && m_DPI > m_XHighDPIValue + m_DPIVariation;
}

bool ScreenHelper::isXXXHDPI() const
{
    return m_DPI < m_XXXHighDPIValue + m_DPIVariation && m_DPI > m_XXHighDPIValue + m_DPIVariation;
}

}
