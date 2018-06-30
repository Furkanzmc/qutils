#pragma once
// Qt
#include <QRect>
#include <QColor>
#include <QObject>
#include <QVariantMap>

namespace zmc
{

class iOSNativeUtils;

/*!
 * \class iOSUtils
 * \brief The iOSUtils class provides access to iOS specific features. Under the hood, it uses iOSNativeUtils.
 *
 * It is a better idea to use NativeUtils to access cros-platform functionalities. This will only be compiled for
 * iOS targets.
 */
class iOSUtils : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit iOSUtils(QObject *parent = nullptr);
    ~iOSUtils();

    /*!
     * \brief Shows a native AlertDialog according to the given dialog properties.
     * You can mix the properties for both Android and iOS. Only the related properties will be used by this function.
     *
     * \code
     * // Show an alert sheet with items
     * var properties = {
     *     "title": "Select An Item",
     *     "buttons": [
     *         "Button 1",
     *         "Button 2"
     *     ]
     * };
     *
     * // This will have the same effect on both iOS and Android
     * var properties = {
     *     "title": "Select An Item",
     *     "positive": "Button 2",
     *     "negative": "Button 1",
     *     "buttons": [
     *         "Button 1",
     *         "Button 2"
     *     ]
     * };
     * \endcode
     * \param dialogData
     */
    Q_INVOKABLE void showAlertView(const QVariantMap &dialogProperties);

    /*!
     * \brief Show the default share dialog on iOS. There's no dialog title on iOS.
     * \param text
     */
    Q_INVOKABLE void shareText(const QString &text);

    /*!
     * \brief Shows an action sheet with the given parameters.
     * \param title
     * \param message
     * \param buttons
     * \param rect
     *
     * \a rect is required on iPad devices. \a rect should be the area that this action sheet's center will be.
     */
    Q_INVOKABLE void showActionSheet(const QString &title, const QString &message, const QVariantList &buttons, QRect rect = QRect());

    /*!
     * \brief Dismisses the keyboard.
     */
    Q_INVOKABLE void dismissKeyboard();

    /*!
     * \brief Sets the application's icon badge number. Set to 0 to clear it.
     * \param number
     */
    Q_INVOKABLE void setApplicationIconBadgeNumber(const int &number);

    /*!
     * \brief Returns true If the device running the app is an iPad.
     */
    Q_INVOKABLE bool isiPad() const;

    /*!
     * \brief If Safari services is enabled, opens the link in an in app browser. Otherwise it does nothing.
     * \param url
     */
    Q_INVOKABLE void openSafari(const QString &url);

    /*!
     * \brief See iOSNativeUtils::openGallery.
     */
    Q_INVOKABLE void openGallery();

    /*!
     * \brief See iOSNativeUtils::getDeviceName().
     * \return QString
     */
    Q_INVOKABLE QString getDeviceName() const;

    /*!
     * \brief See iOSNativeUtils::getStatusBarSize().
     * \return QSize
     */
    Q_INVOKABLE QSize getStatusBarSize() const;

    /*!
     * \brief See iOSNativeUtils::getStatusBarColor().
     * \return QColor
     */
    Q_INVOKABLE QColor getStatusBarColor() const;

    /*!
     * \brief See iOSNativeUtils::openAppSettings().
     */
    Q_INVOKABLE void openAppSettings() const;

    /*!
     * \brief See iOSNativeUtils::setStatusBarColor().
     */
    Q_INVOKABLE void setStatusBarColor(const QColor &color);

    /*!
     * \brief Opens the camera.
     */
    void showCamera();

    /*!
     * \brief Only the first instance will be notified of this.
     * \param url
     */
    static void emitOpenedWithURLSignal(QString url);

    /*!
     * \brief Only the first instance will be notified of this.
     * \param url
     */
    static void emitOpenedWithoutURLSignal();

    /*!
     * \brief Returns true if this is the main controller.
     * \return bool
     */
    Q_INVOKABLE bool isMainController() const;

    /*!
     * \brief Set the main controller to this one.
     * \param isMain
     * \return void
     *
     * When an instance is set as main controller, certain signals will only be emitted for this instance.
     * Only one instance can be the main controller. When an instance is set as main controller while there is
     * already an existing main controller, the existing main controller will relinquish its position and the new
     * instance will be the new main controller.
     *
     * The signals that are designated to main controller are:
     *     - openedWithUrl
     *     - openedWithoutUrl
     */
    Q_INVOKABLE void setMainController(bool isMain, bool disableOthers = true);

    /*!
     * \brief Returns the enabled status
     * \return bool
     */
    bool isEnabled() const;

    /*!
     * \brief If m_IsEnabled is set to false, this instance will not receive any signals from the system.
     * \param enabled
     * \return void
     *
     * So, when m_IsEnabled is false, you cannot show an alert dialog or show the date picker.
     * Basically, anything that requires a callback back to you cannot be used.
     *
     * The only exceptions are the openedWithoutURL() and openedWithURL() signals.
     */
    void setEnabled(bool enabled);

    /*!
     * \brief Refer to iOSNativeUtils::isStatusBarVisible()
     * \return bool
     */
    Q_INVOKABLE bool isStatusBarVisible() const;

    /*!
     * \brief Refer to iOSNativeUtils::setStatusBarVisible()
     * \param visible
     * \return void
     */
    Q_INVOKABLE void setStatusBarVisible(bool visible);

signals:

    /*!
     * \brief Emitted when a button in the alert dialog is clicked.
     * \param buttonIndex
     */
    void alertDialogClicked(int buttonIndex);

    /*!
     * \brief Emitted when a button in the action sheet is clicked.
     * \param buttonIndex
     */
    void actionSheetClicked(int buttonIndex);

    /*!
     * \brief Emitted when the keyboard height changes.
     * \param keyboardHeight
     *
     * A height of 0 means that the keyboard is dismissed.
     */
    void keyboardHeightChanged(int keyboardHeight);

    /*!
     * \brief This signal is emitted when the app is opened with a URL.
     * \param url
     *
     * Follow \l {https://developer.android.com/training/app-links/index.html} {this} tutorial for how to implement it
     * into your app.
     */
    void openedWithURL(QString url);

    /*!
     * \brief This is also called for the first instance.
     *
     * This is just a complementary signal. This is emitted If an app is not opened from a URL.
     */
    void openedWithoutURL();

    /*!
     * \brief Emitted when an image is picked with the image picker.
     */
    void imageSelectionCancelled();

    /*!
     * \brief Emitted when an image picker is used to select an image.
     * \param mediaURL
     */
    void imageSelected(const QString &mediaURL);

    /*!
     * \brief Emitted when m_IsMainController is changed.
     */
    void mainControllerChanged();

    /*!
     * \brief Emitted when user succesfully captures a photo.
     * \param capturePath
     */
    void cameraCaptured(const QString &capturePath);

    /*!
     * \brief Emitted when the user cancels a camera capture.
     */
    void cameraCaptureCancelled();

    /*!
     * \brief Emitted when the enabled property changes.
     */
    void enabledChanged();

private:

    /*!
     * \variable static QMap<int, CacheManager *> iOSUtils::m_Instances
     * \brief This variable is used to keep track of all the instances so that we can send signals to all of them.
     */
    static QMap<int, iOSUtils *> m_Instances;
    static QString m_URLOpenedWith;

    /*!
     * \variable iOSUtils::m_InstanceID
     * \brief This variable is initilized in the constructor to the size value of m_Instances.
     *
     * This is used to identify the current instance in m_Instances.
     */
    const int m_InstanceID;

    iOSNativeUtils *m_iOSNative;
    bool m_IsMainController;

private:

    /*!
     * \brief Determines the source of the image (e.g camera or gallery) and emits the associated signals.
     * \param data
     */
    void imagePickerFinishedPickingCallback(const QVariantMap &data);
};

}
