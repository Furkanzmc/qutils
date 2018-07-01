#pragma once
// Qt
#include <QRect>
#include <QColor>
#include <QObject>
// Local
#include "qutils/Macros.h"
#ifdef Q_OS_ANDROID
    #include "qutils/android/AndroidUtils.h"
#endif // Q_OS_ANDRID

namespace zmc
{

#ifdef Q_OS_IOS
    class iOSUtils;
#endif // Q_OS_IOS
#if defined(Q_OS_MACOS) && !defined(Q_OS_IOS)
    class MacOSUtils;
#endif // Q_OS_MACOS

/*!
 * \class NativeUtils
 * \brief NativeUtils class is used as a main source of entry to both Android and iOS features.
 *
 * When a feature is not supported on the current development platform, the feature will be disabled but it will also
 * not cause a compilation error. Prefer using this class instead of AndroidUtils or iOSUtils.
 */
class NativeUtils : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool buttonEventsEnabled READ isButtonEventsEnabled WRITE setButtonEventsEnabled NOTIFY buttonEventsEnabledChanged)
    Q_PROPERTY(QString statusBarColor READ statusBarColor WRITE setStatusBarColor NOTIFY statusBarColorChanged)
    Q_PROPERTY(bool statusBarVisible READ statusBarVisible WRITE setStatusBarVisible NOTIFY statusBarVisibleChanged)

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool ipad READ isIPad CONSTANT)

    Q_PROPERTY(bool mainController READ mainController WRITE setMainController NOTIFY mainControllerChanged)
    Q_PROPERTY(bool immersiveModeEnabled READ immersiveModeEnabled WRITE setImmersiveModeEnabled NOTIFY immersiveModeEnabledChanged)
    Q_PROPERTY(QSize statusBarSize READ statusBarSize CONSTANT)

public:
    explicit NativeUtils(QObject *parent = 0);

    /*!
     * \property NativeUtils::buttonEventsEnabled
     * \brief Enable or disable button events on Android.
     * \return bool
     *
     * This property only has effect on Android.
     */
    bool isButtonEventsEnabled() const;
    void setButtonEventsEnabled(bool enabled);

    /*!
     * \brief Sets the status bar color on Android and iOS.
     * \param color
     *
     * This method works both on iOS and Android. It has no effect on desktop platforms.
     *
     * \sa AndroidUtils::setStatusBarColor(), iOSUtils::setStatusBarColor()
     */
    QString statusBarColor() const;
    Q_INVOKABLE void setStatusBarColor(QColor color);
    Q_INVOKABLE QString getStatusBarColor() const;

    /*!
     * \property NativeUtils::statusBarVisible
     * \brief Returns true If the status bar is visible.
     * \return bool
     *
     * This method only works on iOS and Android.
     */
    bool statusBarVisible() const;
    Q_INVOKABLE void setStatusBarVisible(bool visible);
    Q_INVOKABLE bool isStatusBarVisible() const;

    /*!
     * \property NativeUtils::immersiveModeEnabled
     * \brief Enable or disable immersive mode on Android.
     * \return bool
     *
     * This property is only available on Android. When the property changes, immersiveModeEnabledChanged() signal
     * is emitted.
     */
    bool immersiveModeEnabled() const;
    void setImmersiveModeEnabled(bool visible);

    /*!
     * \property NativeUtils::enabled
     * \brief Enable or disable the underlying iOSUtils or AndroidUtils class.
     * \return bool
     * \sa iOSUtils::setEnabled(), iOSUtils::isEnabled(), AndroidUtils::setEnabled(), AndroidUtils::isEnabled()
     */
    bool enabled() const;
    void setEnabled(bool enabled);

    /*!
     * \property NativeUtils::ipad
     * \brief Returns true If the device is an iPad.
     * \return bool
     */
    bool isIPad() const;

    /*!
     * \property NativeUtils::mainController
     * \brief Returns \c true If this instance is the main controller.
     * \return bool
     * \sa AndroidUtils::mainController(), AndroidUtils::setMainController(), iOSUtils::mainController(), iOSUtils::setMainController()
     */
    bool mainController() const;
    void setMainController(bool isMain);

    /*!
     * \property NativeUtils::statusBarSize
     * \brief Returns the status bar size on iOS.
     * \return QSize
     *
     * This has no effect on desktop platforms.
     *
     * \todo Add android support.
     */
    Q_INVOKABLE QSize statusBarSize() const;

    /*!
     * \brief Share the given text using the default share dialog on Android and iOS.
     * \param dialogTitle
     * \param text
     *
     * This method does nothing on dekstop platforms.
     *
     * \sa AndroidUtils::shareText(), iOSUtils::shareText()
     */
    Q_INVOKABLE void shareText(const QString &dialogTitle, const QString &text);

    /*!
     * \brief Shows a native alert dialog on iOS.
     * \param dialogProperties
     *
     * This method is only available on iOS.
     *
     * \sa iOSUtils::showAlertView()
     */
    Q_INVOKABLE void showAlertDialog(const QVariantMap &dialogProperties);

    /*!
     * \brief Shows the camera for capturing a photo.
     * \param photoName
     *
     * This method is available on both Android and iOS. It does nothing on desktop platforms.
     *
     * \sa AndroidUtils::showCamera(), iOSUtils::showCamera()
     */
    Q_INVOKABLE void showCamera(const QString &photoName);

    /*!
     * \brief showToast
     * \param text
     * \param isLongDuration
     *
     * Shows a native Toast on Android.
     *
     * \sa AndroidUtils::showToast()
     */
    Q_INVOKABLE void showToast(const QString &text, bool isLongDuration);

    /*!
     * \brief Shows an action sheet on iOS.
     * \param title
     * \param message
     * \param buttons
     * \param rect
     *
     * \sa iOSUtils::showActionSheet()
     */
    Q_INVOKABLE void showActionSheet(const QString &title, const QString &message, const QVariantList &buttons, QRect rect = QRect());

    /*!
     * \brief Hides the keyboard from the active text box on iOS and Android.
     *
     * This method has not effect on desktop platforms.
     */
    Q_INVOKABLE void dismissKeyboard();

    /*!
     * \brief Set the badge number on iOS.
     * \param number
     *
     * This method has not effect on Android and desktop platforms.
     */
    Q_INVOKABLE void setApplicationIconBadgeNumber(const unsigned int &number);

    /*!
     * \brief Opens Safari as an in-app browser on iOS.
     * \param url
     * \note This requires that you enable Safari by adding \c safari_services to \c QUTILS_FEATURES
     *
     * This method has not effect on Android and desktop platforms.
     */
    Q_INVOKABLE void openSafari(const QString &url);

    /*!
     * \brief Returns the device model on iOS and Android.
     * \return QString
     *
     * For implementation details, see the related method on \c iOSUtils and \c AndroidUtils.
     * For now, the default return is \c windows on Windows devices and \ macOS on macOS devices.
     *
     * \sa iOSUtils::getDeviceName(), AndroidUtils::getDeviceModel()
     */
    Q_INVOKABLE QString getDeviceModel();

    /*!
     * \brief Opens the app's settings page on iOS.
     *
     * This method has no effect on Android and desktop platforms.
     */
    Q_INVOKABLE void openAppSettings() const;

signals:
#ifdef Q_OS_ANDROID
    /*!
     * \brief Emitted when the back button is pressed on Android.
     * \param event
     *
     * \sa AndroidUtils::backButtonPressed()
     */
    void backButtonPressed(AndroidButtonEvent *event);

    /*!
     * \brief Emitted when the menu button is pressed on Android.
     * \param event
     *
     * \sa AndroidUtils::menuButtonPressed()
     */
    void menuButtonPressed(AndroidButtonEvent *event);
#else
    void backButtonPressed(void *event);
    void menuButtonPressed(void *event);
#endif // Q_OS_ANDRID

    /*!
     * \brief Emitted when a button in the alert dialog is clicked.
     * \param buttonIndex
     * \sa iOSUtils::alertDialogClicked()
     */
    void alertDialogClicked(int buttonIndex);

    /*!
     * \brief Emitted when a camera capture is finished.
     * \param capturePath
     *
     * \sa iOSUtils::cameraCaptured(), AndroidUtils::cameraCaptured()
     */
    void cameraCaptured(const QString &capturePath);

    /*!
     * \brief Emitted when the user cancels the camera.
     * \sa iOSUtils::cameraCaptureCancelled(), AndroidUtils::cameraCaptureCancelled()
     */
    void cameraCaptureCancelled();

    /*!
     * \brief Emitted when a button in the action sheet is pressed.
     * \param buttonIndex
     * \sa iOSUtils::actionSheetClicked(),
     */
    void actionSheetClicked(int buttonIndex);

    /*!
     * \brief Emitted when the keyboard height changes on iOS and Android.
     * \param keyboardHeight
     * \sa iOSUtils::keyboardHeightChanged(), AndroidUtils::keyboardHeightChanged()
     */
    void keyboardHeightChanged(int keyboardHeight);

    /*!
     * \brief Emitted when the button enabled setting is changed.
     *
     * This is only emitted on Android.
     *
     * \sa AndroidUtils::buttonEventsEnabledChanged()
     */
    void buttonEventsEnabledChanged();

    /*!
     * \brief Emitted when the \c enabled property changes.
     *
     * \sa AndroidUtils::enabledChanged(), iOSUtils::enabledChanged()
     */
    void enabledChanged();

    /*!
     * \brief Emitted when the app is launched with a URL.
     * \param url
     *
     * This is only emitted on iOS and Android.
     *
     * \sa iOSUtils::openedWithURL(), AndroidUtils::openedWithURL()
     */
    void openedWithURL(const QString &url);

    /*!
     * \brief Emitted when the app launched normally.
     *
     * This is only emitted on iOS and Android.
     *
     * \sa iOSUtils::openedWithoutURL(), AndroidUtils::openedWithoutURL()
     */
    void openedWithoutURL();

    /*!
     * \brief Emitted when the \c mainController property changes.
     *
     * \sa iOSUtils::mainControllerChanged(), AndroidUtils::mainControllerChanged(), MacOSUtils::mainControllerChanged()
     */
    void mainControllerChanged();

    /*!
     * \brief Emitted when the status bar color was changed with this instance.
     *
     * \sa iOSUtils::statusBarColorChanged(), AndroidUtils::statusBarColorChanged()
     */
    void statusBarColorChanged();

    /*!
     * \brief Emitted when the status bar visibility was changed with this instance.
     *
     * \sa iOSUtils::statusBarVisibleChanged(), AndroidUtils::statusBarVisibleChanged()
     */
    void statusBarVisibleChanged();

    /*!
     * \brief Emitted when the immersive mode was changed with this instance.
     *
     * \sa AndroidUtils::immersiveModeEnabledChanged()
     */
    void immersiveModeEnabledChanged();

private:
#if defined(Q_OS_ANDROID)
    AndroidUtils *m_AndroidUtils;
#elif defined(Q_OS_IOS)
    iOSUtils *m_iOSUtils;
#elif defined(Q_OS_MACOS) && !defined(Q_OS_IOS)
    MacOSUtils *m_MacOSUtils;
#endif // Platform Check
};

}
