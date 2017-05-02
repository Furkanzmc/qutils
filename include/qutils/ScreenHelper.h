#pragma once
#include <QObject>

namespace zmc
{

/**
 * @brief ScreenHelper is a utility class to help create resolution indepentant UI in QML.
 * When using different icons for different screen densities. If `QUTILS_FOR_MOBILE` macro defined, the ratio is calculated for the values that were given
 * int the constructor. This way, you can simulate different screen sizes on your desktop compiled app.
 * @code
 *     // in main.cpp
 *     ScreenHelper manager;
 *     engine.rootContext()->setContextProperty("SH", &manager);
 * @endcode
 *
 * Then use like so:
 * @code
 *     Window {
 *         id: mainWindow
 *
 *         Rectangle {
 *             width: SH.dp(50)
 *         }
 *     }
 * @endcode
 */
class ScreenHelper : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(bool ldpi READ isLDPI)
    Q_PROPERTY(bool mdpi READ isMDPI)
    Q_PROPERTY(bool hdpi READ isHDPI)

    Q_PROPERTY(bool xhdpi READ isXHDPI)
    Q_PROPERTY(bool xxhdpi READ isXXHDPI)
    Q_PROPERTY(bool xxxhdpi READ isXXXHDPI)

    Q_PROPERTY(float desiredWidth READ getDesiredWidth)
    Q_PROPERTY(float desiredHeight READ getDesiredHeight)

public:
    ScreenHelper(float _dpi = 386.f, float _width = 1080.f, float _height = 1920.f, QObject *parent = nullptr);

    Q_INVOKABLE qreal dp(const qreal &size);
    Q_INVOKABLE qreal fp(const qreal &size);

    Q_INVOKABLE QString getLowResourceFolderName() const;
    void setLowResourceFolderName(const QString &resourceName);

    Q_INVOKABLE QString getMediumResourceFolderName() const;
    void setMediumResourceFolderName(const QString &resourceName);

    Q_INVOKABLE QString getHighResourceFolderName() const;
    void setHighResourceFolderName(const QString &resourceName);

    Q_INVOKABLE QString getXHighResourceFolderName() const;
    void setXHighResourceFolderName(const QString &resourceName);

    Q_INVOKABLE QString getXXHighResourceFolderName() const;
    void setXXHighResourceFolderName(const QString &resourceName);

    Q_INVOKABLE QString getXXXHighResourceFolderName() const;
    void setXXXHighResourceFolderName(const QString &resourceName);

    Q_INVOKABLE QString getResourceFolderName() const;

    /**
     * @brief Returns the resource in the current DPI path.
     * @param fileName
     * @return Resrouce path
     */
    Q_INVOKABLE QString getResource(const QString &fileName) const;

    Q_INVOKABLE float getDesiredHeight() const;
    Q_INVOKABLE float getDesiredWidth() const;

private:
    const qreal m_DPI;
    const unsigned int m_LowDPIValue,
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
            m_XXXHighResourceName;

    float m_Ratio,
          m_RatioFont,
          m_DesiredHeight,
          m_DesiredWidth;

private:
    bool isLDPI() const;
    bool isMDPI() const;
    bool isHDPI() const;

    bool isXHDPI() const;
    bool isXXHDPI() const;
    bool isXXXHDPI() const;

    /**
     * @brief Get the scale that gives a size with preserved aspect ratio.
     * @param origSize The original size of the image
     * @param newSize This is the size you want it to be. Only one property used according to the useHeight parameter.
     * @param useHeight If you want to scale the image according to its height, set to true. Set to false otherwise.
     * @return
     */
    float getAspectRatioWidth(const QSize &origSize, const float &newHeight) const;

};

}
