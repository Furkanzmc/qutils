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

ScreenHelper::ScreenHelper(float _dpi, float _width, float _height, QObject *parent)
    : QObject(parent)
    , m_DPI(QGuiApplication::primaryScreen()->physicalDotsPerInch())
    , m_LowDPIValue(120)
    , m_MediumDPIValue(160)
    , m_HighDPIValue(240)
    , m_XHighDPIValue(320)
    , m_XXHighDPIValue(480)
    , m_XXXHighDPIValue(640)
    , m_DPIVariation(20)
    , m_Ratio(1.f)
    , m_RatioFont(1.f)
    , m_DesiredWidth(0.f)
    , m_DesiredHeight(0.f)
{
#ifdef QT_DEBUG
    qDebug() << "The DPI is: " << m_DPI;
#endif // QT_DEBUG

    // The code here is based on the code provided by Qt here: http://doc.qt.io/qt-5/scalability.html
    const float refDpi = _dpi;
    const float refWidth = _width;
    const float refHeight = _height;

#if defined(Q_OS_DESKTOP) && defined(QUTILS_FOR_MOBILE)
    m_DesiredHeight = QGuiApplication::primaryScreen()->geometry().height() * 0.9f;
    m_DesiredWidth = getAspectRatioWidth(QSize(refWidth, refHeight), m_DesiredHeight);
    const float dpi = refDpi;
    const QRect rect(0, 0, m_DesiredWidth, m_DesiredHeight);
#else
    const QRect rect = QGuiApplication::primaryScreen()->geometry();
    const float dpi = QGuiApplication::primaryScreen()->physicalDotsPerInch();
#endif // Q_OS_DESKTOP

    const float height = qMax(rect.width(), rect.height());
    const float width = qMin(rect.width(), rect.height());
    m_Ratio = qMin(height / refHeight, width / refWidth);
    m_RatioFont = qMin(height * refDpi / (dpi * refHeight), width * refDpi / (dpi * refWidth));
}

qreal ScreenHelper::dp(const qreal &size)
{
    return qMax(2, int(size * m_Ratio));;
}

qreal ScreenHelper::fp(const qreal &size)
{
    return int(size * m_RatioFont);
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

float ScreenHelper::getDesiredHeight() const
{
    return m_DesiredHeight;
}

float ScreenHelper::getDesiredWidth() const
{
    return m_DesiredWidth;
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

float ScreenHelper::getAspectRatioWidth(const QSize &origSize, const float &newHeight) const
{
    // Width Formula: original height / original width * new width = new height
    // Height Formula: orignal width / orignal height * new height = new width

    return newHeight / (static_cast<float>(origSize.height()) / static_cast<float>(origSize.width()));
}

}
