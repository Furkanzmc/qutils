#include "qutils/ScreenHelper.h"
// Qt
#include <QGuiApplication>
#include <QScreen>
#include <QFile>
#ifdef QT_DEBUG
#include <QDebug>
#endif // QT_DEBUG
#include <cmath>
// zmc
#include "qutils/Macros.h"

namespace zmc
{

ScreenHelper::ScreenHelper(const float &refDpi, const float &refWidth, const float &refHeight, const float &refSizeInInches, QObject *parent)
    : QObject(parent)
    , m_ScreenRect(QGuiApplication::primaryScreen()->geometry())
    , m_RefSize(refWidth, refHeight)
    , m_RefDPI(refDpi)
#if defined(Q_OS_DESKTOP) && defined(QUTILS_FOR_MOBILE)
    , m_DPI(refDpi)
#else
    , m_DPI(QGuiApplication::primaryScreen()->physicalDotsPerInch())
#endif // defined(Q_OS_DESKTOP) && defined(QUTILS_FOR_MOBILE)
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
    , m_SizeInInches(refSizeInInches)
    , m_Scale(1.f)
{
    calculateRatio();
    printScreenInfo();

    connect(QGuiApplication::primaryScreen(), &QScreen::orientationChanged, this, &ScreenHelper::calculateRatio);
}

qreal ScreenHelper::dp(const qreal &size)
{
    return qMax(1, int(size * m_Ratio));
}

qreal ScreenHelper::sp(const qreal &size)
{
    return qMax(1, int(size * m_RatioFont));
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

void ScreenHelper::setCommonAssetFolder(const QString &path)
{
    m_CommonAssetFolder = path;
}

QString ScreenHelper::getCommonAssetFolder() const
{
    return m_CommonAssetFolder;
}

QString ScreenHelper::getResourceFolderName() const
{
    QString name = "";

#if defined(Q_OS_DESKTOP) && defined(QUTILS_FOR_MOBILE)
    // If we are on the desktop testing for mobile, use the low DPI assets.
    name = getLowResourceFolderName();
#else
    if (isLDPI()) {
        name = getLowResourceFolderName();
    }
    else if (isMDPI()) {
        name = getMediumResourceFolderName();
    }
    else if (isHDPI()) {
#ifdef Q_OS_MOBILE
        // This is checked in case we are on a low resolution but high DPI device. Like Samsung GT-I9190
        if (isSmallSize()) {
            name = getLowResourceFolderName();
        }
        else if (isNormalSize()) {
            name = getMediumResourceFolderName();
        }
        else {
            name = getHighResourceFolderName();
        }
#else
        name = getHighResourceFolderName();
#endif // Q_OS_MOBILE
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
#endif // defined(Q_OS_DESKTOP) && defined(QUTILS_FOR_MOBILE)

    return name;
}

QString ScreenHelper::getResource(const QString &fileName) const
{
    QString path = getResourceFolderName() + "/" + fileName;
    // Remove the qrc prefix.
    QString pathTemp = path;
    pathTemp.replace("qrc", "");

    if (QFile::exists(pathTemp) == false) {
        path = getCommonAssetFolder() + "/" + fileName;
    }

    return path;
}

float ScreenHelper::getDesiredHeight() const
{
    return m_DesiredHeight;
}

float ScreenHelper::getDesiredWidth() const
{
    return m_DesiredWidth;
}

float ScreenHelper::getSizeInInches() const
{
    return m_SizeInInches;
}

bool ScreenHelper::isLDPI() const
{
    return (m_DPI / m_MediumDPIValue) <= (m_LowDPIValue / m_MediumDPIValue);
}

bool ScreenHelper::isMDPI() const
{
    return (m_DPI / m_MediumDPIValue) > (m_LowDPIValue / m_MediumDPIValue) && (m_DPI / m_MediumDPIValue) < (m_HighDPIValue / m_MediumDPIValue);
}

bool ScreenHelper::isHDPI() const
{
    return (m_DPI / m_MediumDPIValue) >= (m_HighDPIValue / m_MediumDPIValue) && (m_DPI / m_MediumDPIValue) < (m_XHighDPIValue / m_MediumDPIValue);
}

bool ScreenHelper::isXHDPI() const
{
    return (m_DPI / m_MediumDPIValue) >= (m_XHighDPIValue / m_MediumDPIValue) && (m_DPI / m_MediumDPIValue) < (m_XXHighDPIValue / m_MediumDPIValue);
}

bool ScreenHelper::isXXHDPI() const
{
    return (m_DPI / m_MediumDPIValue) >= (m_XXHighDPIValue / m_MediumDPIValue) && (m_DPI / m_MediumDPIValue) < (m_XXXHighDPIValue / m_MediumDPIValue);
}

bool ScreenHelper::isXXXHDPI() const
{
    return (m_DPI / m_MediumDPIValue) >= (m_XXXHighDPIValue / m_MediumDPIValue);
}

bool ScreenHelper::isSmallSize() const
{
    return m_SizeInInches <= 4.6f;
}

bool ScreenHelper::isNormalSize() const
{
    return m_SizeInInches > 4.6f && m_SizeInInches <= 5.3f;
}

bool ScreenHelper::isLargeSize() const
{
    return m_SizeInInches > 5.3f && m_SizeInInches < 6.0f;
}

bool ScreenHelper::isXLargeSize() const
{
    return m_SizeInInches >= 6.0f;
}

void ScreenHelper::calculateRatio()
{
    // The code here is based on the code provided by Qt here: http://doc.qt.io/qt-5/scalability.html
#if defined(Q_OS_DESKTOP) && defined(QUTILS_FOR_MOBILE)
    m_DesiredHeight = qMin((float)m_RefSize.height(), (float)QGuiApplication::primaryScreen()->geometry().height() * 0.8f);
    m_DesiredWidth = getAspectRatioWidth(m_RefSize, m_DesiredHeight);
#else
    const QSizeF physicalSize = QGuiApplication::primaryScreen()->physicalSize();
    m_SizeInInches = std::sqrt(std::pow(physicalSize.width(), 2) + std::pow(physicalSize.height(), 2)) * 0.0393701f;
#endif // Q_OS_DESKTOP

#if defined(Q_OS_DESKTOP) && defined(QUTILS_FOR_MOBILE)
    m_Scale = qMax(m_DesiredWidth / float(m_RefSize.width()), m_DesiredHeight / float(m_RefSize.height()));
#else
    m_Scale = 1.f;
#endif // defined(Q_OS_DESKTOP) && defined(QUTILS_FOR_MOBILE)
#ifdef Q_OS_ANDROID
    const float baseDPI = 160.f;
#else
    const float baseDPI = 163.f;
#endif // Q_OS_ANDROID
    m_Ratio = (m_DPI / baseDPI) * m_Scale;
    m_RatioFont = (m_DPI / baseDPI) * m_Scale;
}

float ScreenHelper::getAspectRatioWidth(const QSize &origSize, const float &newHeight) const
{
    // Width Formula: original height / original width * new width = new height
    // Height Formula: orignal width / orignal height * new height = new width

    return newHeight / (static_cast<float>(origSize.height()) / static_cast<float>(origSize.width()));
}

void ScreenHelper::printScreenInfo() const
{
    LOG("\n----- Screen Info -----" << "\n"
        << "Size in Inches: " << m_SizeInInches << "\n"
        << "DPI: " << m_DPI << "\n"
        << "isSmallSize: " << isSmallSize() << "\n"
        << "isNormalSize: " << isNormalSize() << "\n"
        << "isLargeSize: " << isLargeSize() << "\n"
        << "isXLargeSize: " << isXLargeSize() << "\n"
        << "isLDPI: " << isLDPI() << "\n"
        << "isMDPI: " << isMDPI() << "\n"
        << "isHDPI: " << isHDPI() << "\n"
        << "isXHDPI: " << isXHDPI() << "\n"
        << "isXXHDPI: " << isXXHDPI() << "\n"
        << "isXXXHDPI: " << isXXXHDPI() << "\n"
        << "Ratio: " << m_Ratio << "\n"
        << "RatioFont: " << m_RatioFont << "\n"
        << "----- Screen Info End -----");
}

}
