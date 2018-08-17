#pragma once
// Qt
#include <QRect>
#include <QColor>
#include <QObject>
#include <QVariantMap>

namespace zmc
{

// Forward Decleration
class iOSNativeUtils;

/*!
 * \class iOSUtils
 * \brief The iOSUtils class provides access to iOS specific features. Under the hood, it uses iOSNativeUtils.
 *
 * It is a better idea to use NativeUtils to access cros-platform functionalities. This will only be compiled for
 * iOS targets. This class is not meant to be used in QML.
 */
class iOSUtils : public QObject
{
    Q_OBJECT

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
     *
     * \sa iOSNativeUtils::showAlertView()
     */
    void showAlertView(const QVariantMap &dialogProperties);

    /*!
     * \brief See iOSNativeUtils::shareText
     * \param text
     * \sa iOSNativeUtils::shareText()
     */
    void shareText(const QString &text);

    /*!
     * \brief See iOSNativeUtils::showActionSheet()
     * \param title
     * \param message
     * \param buttons
     * \param rect
     * \sa iOSNativeUtils::showActionSheet()
     */
    void showActionSheet(const QString &title, const QString &message, const QVariantList &buttons, QRect rect = QRect());

    /*!
     * \brief See iOSNativeUtils::dismissKeyboard()
     * \sa iOSNativeUtils::dismissKeyboard()
     */
    void dismissKeyboard();

    /*!
     * \brief See iOSNativeUtils::setApplicationIconBadgeNumber()
     * \param number
     * \sa iOSNativeUtils::setApplicationIconBadgeNumber()
     */
    void setApplicationIconBadgeNumber(const int &number);

    /*!
     * \brief See iOSNativeUtils::isiPad()
     * \sa iOSNativeUtils::isiPad()
     */
    bool isiPad() const;

    /*!
     * \brief See iOSNativeUtils::openSafari()
     * \param url
     * \sa iOSNativeUtils::openSafari()
     */
    void openSafari(const QString &url);

    /*!
     * \brief See iOSNativeUtils::openGallery().
     * \sa iOSNativeUtils::openGallery()
     */
    void openGallery();

    /*!
     * \brief See iOSNativeUtils::getDeviceName().
     * \return QString
     * \sa iOSNativeUtils::getDeviceName()
     */
    QString getDeviceName() const;

    /*!
     * \brief See iOSNativeUtils::getStatusBarSize().
     * \return QSize
     * \sa iOSNativeUtils::getStatusBarSize()
     */
    QSize getStatusBarSize() const;

    /*!
     * \brief See iOSNativeUtils::getStatusBarColor().
     * \return QColor
     * \sa iOSNativeUtils::getStatusBarColor()
     */
    QColor getStatusBarColor() const;

    /*!
     * \brief See iOSNativeUtils::openAppSettings().
     * \sa iOSNativeUtils::openAppSettings()
     */
    void openAppSettings() const;

    /*!
     * \brief See iOSNativeUtils::setStatusBarColor().
     * \sa iOSNativeUtils::setStatusBarColor()
     */
    void setStatusBarColor(const QColor &color);

    /*!
     * \brief See iOSNativeUtils::showCamera().
     * \sa iOSNativeUtils::showCamera()
     */
    void showCamera();

    /*!
     * \brief Returns true if this is the main controller.
     * \return bool
     */
    bool isMainController() const;

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
    void setMainController(bool isMain, bool disableOthers = true);

    /*!
     * \brief Returns true If the instance is enabled.
     * \return bool
     *
     * If enabled is set to false, this instance will not receive any signals from the system.
     * So, when m_IsEnabled is false, you cannot show an alert dialog or show the date picker.
     * Basically, anything that requires a callback back to you cannot be used.
     *
     * The only exceptions are the openedWithoutURL() and openedWithURL() signals.
     */
    bool isEnabled() const;

    /*!
     * \brief Enable or disable the instance.
     * \param enabled
     */
    void setEnabled(bool enabled);

    /*!
     * \brief See iOSNativeUtils::isStatusBarVisible().
     * \return bool
     * \sa iOSNativeUtils::isStatusBarVisible()
     */
    bool isStatusBarVisible() const;

    /*!
     * \brief See iOSNativeUtils::setStatusBarVisible().
     * \param visible
     * \sa iOSNativeUtils::setStatusBarVisible()
     */
    void setStatusBarVisible(bool visible);

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

    /*!
     * \brief Emitted when the status bar visibility changes.
     */
    void statusBarVisibleChanged();

    /*!
     * \brief Emitted when the status bar color changes.
     */
    void statusBarColorChanged();

private:

    /*!
     * \internal
     * \variable static QMap<int, CacheManager *> iOSUtils::m_Instances
     * \brief This variable is used to keep track of all the instances so that we can send signals to all of them.
     */
    static QMap<int, iOSUtils *> m_Instances;

    /*!
     * \variable static unsigned int m_NextInstanceID
     * \brief Used to prevent key clashes in m_Instances.
     */
    static unsigned int m_NextInstanceID;

    static QString m_URLOpenedWith;

    /*!
     * \internal
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
     * \internal
     * \brief Determines the source of the image (e.g camera or gallery) and emits the associated signals.
     * \param data
     */
    void imagePickerFinishedPickingCallback(const QVariantMap &data);
};

}
