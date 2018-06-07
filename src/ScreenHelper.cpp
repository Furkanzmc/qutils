#include "qutils/ScreenHelper.h"
// std
#include <cmath>
// Qt
#include <QGuiApplication>
#include <QScreen>
#include <QFile>
// Local
#include "qutils/Macros.h"

namespace zmc
{

ScreenHelper::ScreenHelper(const float &refDpi, const float &refWidth, const float &refHeight, const float &refSizeInInches, QObject *parent)
    : QObject(parent)
    , m_ScreenRect(QGuiApplication::primaryScreen()->geometry())
    , m_RefSize(refWidth, refHeight)
    , m_RefDPI(refDpi)
#if defined(Q_OS_DESKTOP) && QUTILS_FOR_MOBILE == 1
    , m_DPI(refDpi)
#else
    , m_DPI(QGuiApplication::primaryScreen()->physicalDotsPerInch())
#endif // defined(Q_OS_DESKTOP) && QUTILS_FOR_MOBILE == 1
#if defined(Q_OS_DESKTOP)
    , m_LowDPIValue(96)
    , m_MediumDPIValue(113)
    , m_HighDPIValue(197)
    , m_XHighDPIValue(227)
    , m_XXHighDPIValue(267)
    , m_XXXHighDPIValue(267)
#else
    , m_LowDPIValue(120)
    , m_MediumDPIValue(160)
    , m_HighDPIValue(240)
    , m_XHighDPIValue(320)
    , m_XXHighDPIValue(480)
    , m_XXXHighDPIValue(640)
#endif // Q_OS_DESKTOP
    , m_DPIVariation(20)
    , m_Ratio(1.f)
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
    return qMax(1, static_cast<int>(size * m_Ratio));
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

#if defined(Q_OS_DESKTOP) && QUTILS_FOR_MOBILE == 1
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
#endif // defined(Q_OS_DESKTOP) && QUTILS_FOR_MOBILE == 1

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

float ScreenHelper::dpi() const
{
    return  m_DPI;
}

float ScreenHelper::ldpi() const
{
    return m_LowDPIValue;
}

float ScreenHelper::mdpi() const
{
    return m_MediumDPIValue;
}

float ScreenHelper::hdpi() const
{
    return m_HighDPIValue;
}

float ScreenHelper::xhdpi() const
{
    return m_XHighDPIValue;
}

float ScreenHelper::xxhdpi() const
{
    return m_XXHighDPIValue;
}

float ScreenHelper::xxxhdpi() const
{
    return m_XXXHighDPIValue;
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
#if defined(Q_OS_MOBILE)
    return m_SizeInInches <= 4.6f;
#elif defined(Q_OS_DESKTOP)
    return m_SizeInInches <= 13.f;
#endif // Q_OS_MOBILE
}

bool ScreenHelper::isNormalSize() const
{
#if defined(Q_OS_MOBILE)
    return m_SizeInInches > 4.6f && m_SizeInInches <= 5.3f;
#elif defined(Q_OS_DESKTOP)
    return m_SizeInInches > 13.f && m_SizeInInches <= 15.f;
#endif // Q_OS_MOBILE
}

bool ScreenHelper::isLargeSize() const
{
#if defined(Q_OS_MOBILE)
    return m_SizeInInches > 5.3f && m_SizeInInches < 6.0f;
#elif defined(Q_OS_DESKTOP)
    return m_SizeInInches > 15.f && m_SizeInInches < 18.0f;
#endif // Q_OS_MOBILE
}

bool ScreenHelper::isXLargeSize() const
{
#if defined(Q_OS_MOBILE)
    return m_SizeInInches >= 6.0f;
#elif defined(Q_OS_DESKTOP)
    return m_SizeInInches >= 18.0f;
#endif // Q_OS_MOBILE
}

float ScreenHelper::ratio() const
{
    return m_Ratio;
}

void ScreenHelper::calculateRatio()
{
#if defined(Q_OS_DESKTOP) && QUTILS_FOR_MOBILE
    const QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    m_DesiredHeight = qMin(static_cast<float>(m_RefSize.height()), static_cast<float>(screenGeometry.height()) * 0.8f);
    m_DesiredWidth = getAspectRatioWidth(m_RefSize, m_DesiredHeight);
#elif defined(Q_OS_DESKTOP)
    const QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    m_DesiredHeight = qMin(static_cast<float>(m_RefSize.height()), static_cast<float>(screenGeometry.height()) * 0.8f);
    m_DesiredWidth = getAspectRatioWidth(m_RefSize, m_DesiredHeight);
#elif !defined(Q_OS_MOBILE)
#error "Cannot set a desired size for the current platform."
#endif // Q_OS_DESKTOP

    const QSizeF physicalSize = QGuiApplication::primaryScreen()->physicalSize();
    m_SizeInInches = std::sqrt(std::pow(physicalSize.width(), 2) + std::pow(physicalSize.height(), 2)) * 0.0393701f;

#if defined(Q_OS_DESKTOP) && QUTILS_FOR_MOBILE == 1
    m_Scale = qMax(m_DesiredWidth / float(m_RefSize.width()), m_DesiredHeight / float(m_RefSize.height()));
#else
    m_Scale = 1.f;
#endif // defined(Q_OS_DESKTOP) && QUTILS_FOR_MOBILE == 1

#if defined(Q_OS_ANDROID)
    const float baseDPI = 160.f;
#elif defined(Q_OS_IOS)
    const float baseDPI = 163.f;
#elif defined(Q_OS_WIN) || defined(Q_OS_WINRT)
    const float baseDPI = 96.f;
#elif defined(Q_OS_MACOS) || defined(Q_OS_UNIX)
    const float baseDPI = 113.5f;
#endif // Q_OS_ANDROID
    m_Ratio = (m_DPI / baseDPI) * m_Scale;
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
        << "Actual DPI: " << QGuiApplication::primaryScreen()->physicalDotsPerInch() << "\n"
        << "----- Screen Info End -----");
}

}
