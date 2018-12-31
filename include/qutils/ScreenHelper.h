#pragma once
// Qt
#include <QObject>
#include <QRect>

// Forward Declerations
class QQmlEngine;
class QJSEngine;

namespace zmc
{

/*!
 * \class ScreenHelper
 * \brief ScreenHelper is a utility class to help create resolution indepentant UI in QML.
 *
 * When using different icons for different screen densities. If `QUTILS_FOR_MOBILE` macro defined, the ratio is calculated for the values that were given
 * int the constructor. This way, you can simulate different screen sizes on your desktop compiled app.
 *
 * When calculating the ratio, the following base DPIs are used:
 *     - 160 for Android
 *     - 163 for iOS
 *     - 113.5 for macOS
 *     - 96 for Windows
 *
 * \code
 *     // in main.cpp
 *     ScreenHelper manager;
 *     engine.rootContext()->setContextProperty("SH", &manager);
 * \endcode
 *
 * Then use like so:
 * \code
 *     Window {
 *         id: mainWindow
 *
 *         Rectangle {
 *             width: ScreenHelper.dp(50)
 *         }
 *     }
 * \endcode
 */
class ScreenHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float dpi READ dpi CONSTANT)
    Q_PROPERTY(float ldpi READ ldpi CONSTANT)
    Q_PROPERTY(float mdpi READ mdpi CONSTANT)

    Q_PROPERTY(float hdpi READ hdpi CONSTANT)
    Q_PROPERTY(float xhdpi READ xhdpi CONSTANT)
    Q_PROPERTY(float xxhdpi READ xxhdpi CONSTANT)

    Q_PROPERTY(float xxxhdpi READ xxxhdpi CONSTANT)
    Q_PROPERTY(float desiredWidth READ getDesiredWidth CONSTANT)
    Q_PROPERTY(float desiredHeight READ getDesiredHeight CONSTANT)

    Q_PROPERTY(float sizeInInches READ getSizeInInches CONSTANT)
    Q_PROPERTY(bool small READ isSmallSize CONSTANT)
    Q_PROPERTY(bool normal READ isNormalSize CONSTANT)

    Q_PROPERTY(bool large READ isLargeSize CONSTANT)
    Q_PROPERTY(bool xlarge READ isXLargeSize CONSTANT)
    Q_PROPERTY(float ratio READ ratio CONSTANT)

public:

    /*!
     * \brief The default constructor for ScreenHelper.
     * \param refDpi
     * \param refWidth
     * \param refHeight
     * \param refSizeInInches
     * \param parent
     *
     * Ideally, you should provide different reference sizes for each platform.
     */
    ScreenHelper(QObject *parent = nullptr);

    /*!
     * \brief The singleton provider for Cuz class.
     * \return QObject *
     */
    static QObject *singletonProvider(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    /*!
     * \brief This method takes the \a size and returns a DPI version.
     * \param size
     * \return int
     */
    Q_INVOKABLE int dp(const int &size);

    /*!
     * \brief Returns the assets folder name for a low DPI screen.
     * \return QString
     */
    QString getLowResourceFolderName() const;

    /*!
     * \brief Set the folder name for the low DPI screens.
     * \param resourceName
     *
     * This should be an absolute path to the folder.
     *
     * \code
     *     ScreenHelper sh;
     *     sh.setLowResourceFolderName("qrc:/assets/low");
     * \endcode
     */
    void setLowResourceFolderName(const QString &resourceName);

    /*!
     * \brief Returns the assets folder name for a medium DPI screen.
     * \return QString
     */
    QString getMediumResourceFolderName() const;

    /*!
     * \brief Set the folder name for the medium DPI screens.
     * \param resourceName
     *
     * This should be an absolute path to the folder.
     *
     * \code
     *     ScreenHelper sh;
     *     sh.setMediumResourceFolderName("qrc:/assets/medium");
     * \endcode
     */
    void setMediumResourceFolderName(const QString &resourceName);

    /*!
     * \brief Returns the assets folder name for a high DPI screen.
     * \return QString
     */
    QString getHighResourceFolderName() const;

    /*!
     * \brief Set the folder name for the high DPI screens.
     * \param resourceName
     *
     * This should be an absolute path to the folder.
     *
     * \code
     *     ScreenHelper sh;
     *     sh.setHighResourceFolderName("qrc:/assets/high");
     * \endcode
     */
    void setHighResourceFolderName(const QString &resourceName);

    /*!
     * \brief Returns the assets folder name for a extra high DPI screen.
     * \return QString
     */
    QString getXHighResourceFolderName() const;

    /*!
     * \brief Set the folder name for the xhigh DPI screens.
     * \param resourceName
     *
     * This should be an absolute path to the folder.
     *
     * \code
     *     ScreenHelper sh;
     *     sh.setXHighResourceFolderName("qrc:/assets/xhigh");
     * \endcode
     */
    void setXHighResourceFolderName(const QString &resourceName);

    /*!
     * \brief Returns the assets folder name for a xx-high DPI screen.
     * \return QString
     */
    QString getXXHighResourceFolderName() const;

    /*!
     * \brief Set the folder name for the xxhigh DPI screens.
     * \param resourceName
     *
     * This should be an absolute path to the folder.
     *
     * \code
     *     ScreenHelper sh;
     *     sh.setXXHighResourceFolderName("qrc:/assets/xxhigh");
     * \endcode
     */
    void setXXHighResourceFolderName(const QString &resourceName);

    /*!
     * \brief Returns the assets folder name for a xxx-high DPI screen.
     * \return QString
     */
    QString getXXXHighResourceFolderName() const;

    /*!
     * \brief Set the folder name for the xxxhigh DPI screens.
     * \param resourceName
     *
     * This should be an absolute path to the folder.
     *
     * \code
     *     ScreenHelper sh;
     *     sh.setXXXHighResourceFolderName("qrc:/assets/xxxhigh");
     * \endcode
     */
    void setXXXHighResourceFolderName(const QString &resourceName);

    /*!
     * \brief This folder is used when the asset cannot be found in the folder assigned to the current DPI.
     * \param path
     * \return void
     */
    void setCommonAssetFolder(const QString &path);

    /*!
     * \brief Returns the path to the common assets folder.
     * \return
     */
    QString getCommonAssetFolder() const;

    /*!
     * \brief Returns the assets folder name for used for the current screen configuration.
     * \return QString
     */
    QString getResourceFolderName() const;

    /*!
     * \brief Returns the resource in the current DPI path.
     * \param fileName
     * \return Resrouce path
     */
    Q_INVOKABLE QString getResource(const QString &fileName) const;

    /*!
     * \brief Returns the scaled down height from the reference height.
     * \return float
     */
    float getDesiredHeight() const;

    /*!
     * \brief Returns the scaled down width from the reference width.
     * \return float
     */
    float getDesiredWidth() const;

    /*!
     * \brief Returns the size of the primary screen in inches.
     * \return float
     */
    float getSizeInInches() const;

    void setSizeInInches(float size);

    /*!
     * \property ScreenHelper::dpi
     * \brief Returns the DPI of the current screen.
     * \return float
     */
    float dpi() const;

    /*!
     * \property ScreenHelper::ldpi
     * \brief Returns the DPI value for a low DPI screen.
     * \return float
     */
    float ldpi() const;

    /*!
     * \property ScreenHelper::mdpi
     * \brief Returns the DPI value for a medium DPI screen.
     * \return float
     */
    float mdpi() const;

    /*!
     * \property ScreenHelper::hdpi
     * \brief Returns the DPI value for a high DPI screen.
     * \return float
     */
    float hdpi() const;

    /*!
     * \property ScreenHelper::xhdpi
     * \brief Returns the DPI value for a xhigh DPI screen.
     * \return float
     */
    float xhdpi() const;

    /*!
     * \property ScreenHelper::xxhdpi
     * \brief Returns the DPI value for a xxhigh DPI screen.
     * \return float
     */
    float xxhdpi() const;

    /*!
     * \property ScreenHelper::xxxhdpi
     * \brief Returns the DPI value for a xxxhigh DPI screen.
     * \return float
     */
    float xxxhdpi() const;

    /*!
     * \brief Returns true If the current DPI is within the limits of being a low DPI screen.
     * \return bool
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE bool isLDPI() const;

    /*!
     * \brief Returns true If the current DPI is within the limits of being a medium DPI screen.
     * \return bool
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE bool isMDPI() const;

    /*!
     * \brief Returns true If the current DPI is within the limits of being a high DPI screen.
     * \return bool
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE bool isHDPI() const;

    /*!
     * \brief Returns true If the current DPI is within the limits of being a xhigh DPI screen.
     * \return bool
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE bool isXHDPI() const;

    /*!
     * \brief Returns true If the current DPI is within the limits of being a xxhigh DPI screen.
     * \return bool
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE bool isXXHDPI() const;

    /*!
     * \brief Returns true If the current DPI is within the limits of being a xxxhigh DPI screen.
     * \return bool
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE bool isXXXHDPI() const;

    /*!
     * \brief Returns true If the current screen size is small.
     * \return bool
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE bool isSmallSize() const;

    /*!
     * \brief Returns true If the current screen size is normal size.
     * \return bool
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE bool isNormalSize() const;

    /*!
     * \brief Returns true If the current screen size is large.
     * \return bool
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE bool isLargeSize() const;

    /*!
     * \brief Returns true If the current screen size is extra large.
     * \return bool
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE bool isXLargeSize() const;

    /*!
     * \property ScreenHelper::ratio
     * \brief Returns the ratio used to calculate dp values.
     * \return int
     */
    int ratio() const;

    QSize getRefSize() const;
    void setRefSize(const QSize &size);

    float getRefDPI() const;
    void setRefDPI(const float &refDPI);

private:
    const QRect m_ScreenRect;
    QSize m_RefSize;
    float m_RefDPI, m_DPI;

    const float m_LowDPIValue,
          m_MediumDPIValue,
          m_HighDPIValue,
          m_XHighDPIValue,
          m_XXHighDPIValue,
          m_XXXHighDPIValue,
          m_DPIVariation;

    // You can use these values to automatically append or prepend to your resource name based on the DPI value.
    QString m_LowResourceName,
            m_MediumResourceName,
            m_HighResourceName,
            m_XHighResourceName,
            m_XXHighResourceName,
            m_XXXHighResourceName,
            m_CommonAssetFolder;

    int m_Ratio;
    float m_DesiredWidth,
          m_DesiredHeight,
          m_SizeInInches,
          m_Scale;

private:
    /*!
     * \brief This should only be called once since the screen configuration is unlikely to change.
     */
    void calculateRatio();

    /*!
     * \brief Get the scale that gives a size with preserved aspect ratio.
     * \param origSize The original size of the image
     * \param newSize This is the size you want it to be. Only one property used according to the useHeight parameter.
     * \param useHeight If you want to scale the image according to its height, set to true. Set to false otherwise.
     * \return float
     */
    float getAspectRatioWidth(const QSize &origSize, const float &newHeight) const;

    /*!
     * \brief Prints information about the current screen configuration.
     */
    void printScreenInfo() const;

private:
    static ScreenHelper *m_Instance;
};

}
