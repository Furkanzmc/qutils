#pragma once
// Qt
#include <QColor>
#include <QObject>
#include <QtAndroidExtras/QAndroidJniObject>
// Local
#include "qutils/NotificationClient.h"

namespace zmc
{

/*!
 * \class AndroidButtonEvent
 * \brief The AndroidButtonEvent class can be used to break the button event propagation to the other AndroidUtils classes.
 *
 * If you set accepted to true, the event propagation will be stopped. This class has QML accessible properties.
 */
class AndroidButtonEvent : public QObject
{
    Q_OBJECT Q_PROPERTY(bool accepted READ isAccepted WRITE setAccepted NOTIFY acceptedChanged)

public:
    AndroidButtonEvent(QObject *parent = nullptr);

    /*!
     * \property AndroidButtonEvent::accepted
     * \brief Returns true If the event was accepted by one of the listeners.
     * \return bool
     *
     * If you set accepted to true, the event propagation will be stopped.
     */
    bool isAccepted() const;
    void setAccepted(bool accepted);

signals:

    /*!
     * \brief Emitted when the accepted property changes.
     */
    void acceptedChanged();

private:
    bool m_IsAccepted;
};

/*!
 * \class AndroidUtils
 * \brief The AndroidUtils class contains utility methods for Android.
 *
 * This is included only for Android targets. You should opt in to use NativeUtils as the entry point to native
 * functionalities for a smooth cross platform experience. This class does not support QML usage.
 */
class AndroidUtils : public QObject
{
    Q_OBJECT

public:
    explicit AndroidUtils(QObject *parent = 0);
    ~AndroidUtils();

    /*!
     * \brief Returns the current status bar color as hex color string.
     * \return QString
     */
    QString statusBarColor() const;

    /*!
     * \brief Change the status bar color.
     * \param color
     *
     * This method expects \a color to be a valid color.
     * If the status bar color chanes, emits statusBarColorChanged() signal.
     */
    void setStatusBarColor(QColor color);

    /*!
     * \brief Returns true If the status bar is visible.
     * \return bool
     */
    bool statusBarVisible() const;

    /*!
     * \brief Changes the status bar visibility.
     * \param visible
     *
     * If the visibility changes, emits statusBarVisibleChanged() signal.
     */
    void setStatusBarVisible(bool visible);

    /*!
     * \brief Returns true If the immersive mode is enabled.
     * \return bool
     */
    bool immersiveModeEnabled() const;

    /*!
     * \brief Enable\disable immersive mode.
     * \param enabled
     *
     * If the mode changes, emits immersiveModeEnabledChanged() signal.
     */
    void setImmersiveModeEnabled(bool enabled);

    /*!
     * \brief Shares the given text using Intent.ACTION_SEND with text/plain MIME type.
     * \param dialogTitle
     * \param text
     */
    void shareText(const QString &dialogTitle, const QString &text);

    /*!
     * \brief If m_IsEnabled is false, you cannot use this function.
     * \return void
     */
    void showCamera(const QString &fileName);

    /*!
     * \brief Shows toast with the given text.
     * \param message
     * \param isLongDuration
     * \return void
     */
    void showToast(const QString &message, bool isLongDuration);

    /*!
     * \brief Opens the gallery for an pick file operation.
     * \return void
     *
     * If the instance is disabled, you cannot use this function. The MIME type defaults to image/\*.
     */
    void openGallery();

    /*!
     * \brief Opens the gallery for an pick file operation.
     * \param fileType The MIME type for the type of file to be selected.
     * \return void
     *
     * If the instance is disabled, you cannot use this function. You can split the MIME types with "|" to set multiple
     * MIME types. Example: `application/pdf|image/jpeg|text/plain`.
     */
    void openDocumentPicker(const QString &fileType);

    /*!
     * \brief Dismisses the keyboard from the view.
     * \return void
     */
    void dismissKeyboard();

    /*!
     * \brief Returns the device model
     * \return QString
     */
    QString getDeviceModel() const;

    /*!
     * \brief Returns true If the button events are being handled by this instance.
     * \return bool
     */
    bool isButtonEventsEnabled() const;

    /*!
     * \brief Enable button events to recieve back button and menu button signals.
     * \param enabled
     */
    void setButtonEventsEnabled(bool enabled);

    /*!
     * \brief Returns true If this instance is enabled.
     * \return bool
     */
    bool isEnabled() const;

    /*!
     * \brief Enable disable the instance.
     * \param enabled
     *
     * If enabled is set to false, this instance will not receive any signals from the system. Basically, anything that
     * requires a callback back to you cannot be used when this instance is disabled.
     *
     * The only exceptions are the openedWithoutURL() and openedWithURL() signals. These two signals are always emitted.
     */
    void setEnabled(bool enabled);

    /*!
     * \brief Returns true if this is the main controller.
     * \return bool
     */
    bool mainController() const;

    /*!
     * \brief Set this instance as the main controller.
     * \param isMain
     * \param disableOthers
     *
     * When an instance is set as main controller, certain signals will only be emitted for this instance.
     * Only one instance can be the main controller. When an instance is set as main controller while there is already
     * an existing main controller, the existing main controller will relinquish its position and the new instance will
     * be the new main controller.
     *
     * The signals that are designated to main controller are:
     * \list
     *     \li openedWithUrl
     *     \li openedWithoutUrl
     * \endlist
     */
    void setMainController(bool isMain, bool disableOthers = true);

    /*!
     * \internal
     * \brief Signals are not emitted if the application state is not Qt::ApplicationActive.
     * \param isBackButton
     * \param isMenuButton
     *
     * AndroidUtils starts from the latest created AndroidUtils instance and works its way down to the bottom.
     * And every instance who has the buttonEventsEnabled property true is notified of the signal.
     * The default value of the property is false.
     */
    static void emitButtonPressedSignals(bool isBackButton, bool isMenuButton);

    /*!
     * \internal
     * \brief Emits the camera captured signal for the instance that requested the camera.
     * \param capturePath
     *
     * This is called from JNICallbacks.
     */
    static void emitCameraCapturedSignals(const QString &capturePath);

    /*!
     * \internal
     * \brief Emits the camera canceled signal for the instance that requested the camera.
     *
     * This is called from JNICallbacks.
     */
    static void emitCameraCaptureCancelledSignals();

    /*!
     * \internal
     * \brief Emits the file selected signal for the instance that requested the file selection.
     * \param filePath
     *
     * This is called from JNICallbacks.
     */
    static void emitFileSelectedSignals(const QString &filePath);

    /*!
     * \internal
     * \brief Emits the file selection canceled signal for the instance that requested the file selection.
     *
     * This is called from JNICallbacks.
     */
    static void emitFileSelectionCancelledSignals();

    /*!
     * \internal
     * \brief Emits keyboardHeightChanged signal in all of the instances.
     * \param keyboardHeight
     *
     * This is called from JNICallbacks.
     */
    static void emitKeyboardHeightChangedSignals(const int &keyboardHeight);

    /*!
     * \internal
     * \brief Only the first instance will be notified of this.
     * \param url
     */
    static void emitOpenedWithURLSignal(const QString &url);

    /*!
     * \internal
     * \brief Only the first instance will be notified of this.
     * \param url
     */
    static void emitOpenedWithoutURLSignal();

signals:

    /*!
     * \brief This signaled everytime the back button is pressed.
     *
     * For now, this behaviour overrides the close behaviour of the Window. So you need to manually close it yourself.
     * Only the last instance is informed of the back button signal.
     */
    void backButtonPressed(AndroidButtonEvent *event);

    /*!
     * \brief Only the last instance is informed of the menu button signal.
     */
    void menuButtonPressed(AndroidButtonEvent *event);

    /*!
     * \brief Emitted when the camera capture succeeds.
     * \param capturePath
     */
    void cameraCaptured(const QString &capturePath);

    /*!
     * \brief Emitted when the user cancels the camera capture.
     */
    void cameraCaptureCancelled();

    /*!
     * \brief Emitted when the file selection succeeds.
     * \param filePaths
     */
    void fileSelected(const QStringList &filePaths);

    /*!
     * \brief Emitted when the user cancels the file selection.
     */
    void fileSelectionCancelled();

    /*!
     * \brief Emittedn when the user selects a photo from the gallery.
     * \param filePath
     */
    void photoSelected(const QString &filePath);

    /*!
     * \brief Emitted when the user cancels the photo selection from the gallery.
     */
    void photoSelectionCanceled();

    /*!
     * \brief Emitted when the keyboard height changes.
     * \param keyboardHeight
     */
    void keyboardHeightChanged(int keyboardHeight);

    /*!
     * \brief Emitted when AndroidUtils::buttonEventsEnabled property changes.
     */
    void buttonEventsEnabledChanged();

    /*!
     * \brief Emitted when AndroidUtils::enabled property changes.
     */
    void enabledChanged();

    /*!
     * \brief Emitted when the status bar color changes.
     *
     * Note that this signal is only emitted when the status bar color is changed using the same instance.
     */
    void statusBarColorChanged();

    /*!
     * \brief Emitted when the status bar visibility changes.
     *
     * Note that this signal is only emitted when the status bar color is changed using the same instance.
     */
    void statusBarVisibleChanged();

    /*!
     * \brief Emitted when the immersive mode changes.
     *
     * Note that this signal is only emitted when the status bar color is changed using the same instance.
     */
    void immersiveModeEnabledChanged();

    /*!
     * \brief This signal is emitted when the app is opened with a URL.
     * \param url
     *
     * Follow \l {https://developer.android.com/training/app-links/index.html}{this} tutorial for how to implement it
     * into your app.
     */
    void openedWithURL(const QString &url);

    /*!
     * \brief Emitted when the app opens normally.
     *
     * This is also emitted for the first instance. This is just a complementary signal. This is emitted If an app is
     * not opened from a URL.
     */
    void openedWithoutURL();

    /*!
     * \brief Emitted when AndroidUtils::mainController property is changed.
     */
    void mainControllerChanged();

private:

    /*!
     * \internal
     * \variable static QMap<int, CacheManager *> m_Instances
     * \brief This variable is used to keep track of all the instances so that we can send signals to all of them.
     */
    static std::map<int, AndroidUtils *> m_Instances;
    static QString m_URLOpenedWith;

    /*!
     * \internal
     * \variable const int m_InstanceID
     * \brief This variable is initilized in the constructor to the size value of m_Instances.
     *
     * This is used to identify the current instance in m_Instances.
     */
    const int m_InstanceID;

    bool m_IsCameraShown,
         m_IsGalleryShown,
         m_IsDocumentPickerShown,
         m_IsMainController;
    /*!
     * \internal
     * \brief The default value is false. This is used to determine which instance to send the backButtonPressed signal.
     */
    bool m_IsButtonEventsEnabled;

private:

    /*!
     * \internal
     * \brief Emits the AndroidUtils::backButtonPressed signal if the instance and the button events are enabled.
     * \param event
     */
    void emitBackButtonPressed(AndroidButtonEvent *event);

    /*!
     * \internal
     * \brief Emits the AndroidUtils::menuButtonPressed signal if the instance and the button events are enabled.
     * \param event
     */
    void emitMenuButtonPressed(AndroidButtonEvent *event);

    /*!
     * \brief Emits AndroidUtils::cameraCaptured signal If this is the caller instance.
     * \param capturePath
     */
    void emitCameraCaptured(const QString &capturePath);

    /*!
     * \brief Emits AndroidUtils::cameraCaptureCancelled signal If this is the caller instance.
     */
    void emitCameraCaptureCancelled();

    /*!
     * \brief Emits AndroidUtils::fileSelected signal If this is the caller instance.
     * \param filePath
     */
    void emitFileSelected(QString filePath);

    /*!
     * \brief Emits AndroidUtils::fileSelectionCancelled signal If this is the caller instance.
     */
    void emitFileSelectionCancelled();

    /*!
     * \internal
     * \brief Converts a QVariantMap to HashMap in Java.
     * \param map
     * \return QAndroidJniObject
     *
     * Supported types are:
     * - Integer
     * - Bool
     * - QVariantMap
     * - List (Sadly, only String lists for now)
     */
    QAndroidJniObject getJNIHashMap(const QVariantMap &map) const;
};

}
