#pragma once
// std
#include <functional>
// Qt
#include <QRect>
#include <QColor>
#include <QStringList>
#include <QVariantList>

namespace zmc
{

/*!
 * \class iOSNativeUtils
 * \brief The iOSNativeUtils class is a bridge between Qt and Objective-C++ for accessing native functionalities of iOS.
 */
class iOSNativeUtils
{
public:

    /*!
     * \variable iOSNativeUtils::onAlertDialogClicked
     * \brief Callback for alert dialog.
     *
     * The first and only parameter is the index of the clicked button.
     */
    std::function<void(unsigned int /*buttonIndex*/)> onAlertDialogClicked;

    /*!
     * \variable iOSNativeUtils::onActionSheetClicked
     * \brief Callback for action sheet.
     *
     * The first and only parameter is the index of the clicked button.
     */
    std::function<void(unsigned int /*buttonIndex*/)> onActionSheetClicked;

    /*!
     * \variable iOSNativeUtils::onImagePickerControllerCancelled
     * \brief Callback for when the image picker is canceled by user.
     */
    std::function<void()> onImagePickerControllerCancelled;

    /*!
     * \variable iOSNativeUtils::onCameraCancelled
     * \brief Callback for when the camera is canceled by user.
     */
    std::function<void()> onCameraCancelled;

    /*!
     * \variable iOSNativeUtils::onImagePickerControllerFinishedPicking
     * \brief Callback for image picker.
     *
     * The following keys will be present in the map:
     * \list
     *     \li mediaType: The media type for the file.
     *     \li tempUrl: The file that resides at the temporary location for read and write access by the app.
     *     \li referenceUrl: The reference URL provided by PHImageManager.
     *     \li isSourceCamera: A bool value that indicates If the image was taken from the camera.
     * \endlist
     */
    std::function<void(const QVariantMap &/*info*/)> onImagePickerControllerFinishedPicking;

    /*!
     * \variable iOSNativeUtils::onKeyboardHeightChanged
     * \brief Callback for keyboard height changes.
     *
     * If the keyboard is hidden then the width and height will be zero. Otherwise they will be non-zero.
     * Keyboard's size will be reported as zero with UIKeyboardWillHideNotification. And the keyboard height will
     * change with UIKeyboardWillShowNotification.
     */
    std::function<void(int /*height*/)> onKeyboardHeightChanged;

public:
    iOSNativeUtils();
    ~iOSNativeUtils();

    /*!
     * \brief Shows a native alert view on iOS.
     * \param title
     * \param message
     * \param buttons
     *
     * Make sure that you connect the onAlertDialogClicked lamda to get a callback. Otherwise interaction will not be
     * reported back to you.
     */
    void showAlertView(const QString &title, const QString &message, const QStringList &buttons);

    /*!
     * \brief Show the default share dialog on iOS. There's no dialog title on iOS.
     * \param text
     */
    void shareText(const QString &text);

    /*!
     * \brief buttons format is as follows.
     * [
     *     {
     *         "title": "Button 1",
     *         "type": "cancel"
     *     }
     * ]
     *
     * Button type options are: "cancel", "destructive" and "default". If anything other than these are given, "default" is used.
     * \param title
     * \param message
     * \param buttons
     * \param rect
     *
     * \a rect is required on iPad devices. \a rect should be the area that this action sheet's center will be.
     */
    void showActionSheet(const QString &title, const QString &message, const QVariantList &buttons, QRect rect = QRect());

    /*!
     * \brief This method DOES NOT work yet.
     * \param title
     * \param body
     * \param delayInSeconds
     */
    void schedulePushNotification(const QString &title, const QString &body, const int &delayInSeconds);

    /*!
     * \brief Hides the keyboard.
     * \return void
     */
    void dismissKeyboard();

    /*!
     * \brief Sets the application's icon badge number. Set to 0 to clear it.
     * \param number
     * \return void
     */
    void setApplicationIconBadgeNumber(const int &number);

    /*!
     * \brief Returns true If the device running the app is an iPad.
     * \return bool
     */
    bool isiPad() const;

    /*!
     * \brief Uses iOS SafariServices to open the url in an in-app browser. The browser events are not yet supported.
     * \param url
     * \return void
     *
     * \note This function requires safari_services in QUTILS_FEATURES.
     */
    void openSafari(const QString &url);

    /*!
     * \brief Opens the gallery to pick an image.
     * \return void
     */
    void openGallery();

    /*!
     * \brief Opens the camera. The file name for the capture is automated.
     * \return void
     */
    void showCamera();

    /*!
     * \brief This will be set to true when the image picker is open and only this instance will be notified of the events from the image picker.
     * \return bool
     */
    bool isImagePickerOpen() const;

    /*!
     * \brief This will be true when this instance calls the camera.
     * \return bool
     */
    bool isCameraOpen() const;

    /*!
     * \brief Hide or show the status bar.
     * In order for this to take effect you need to have `UIViewControllerBasedStatusBarAppearance` set to false in your Info.plist file.
     * \param visible
     * \return void
     */
    void setStatusBarVisible(bool visible);

    /*!
     * \brief Returns true If the status bar is hidden.
     * \return bool
     */
    bool isStatusBarVisible() const;

    /*!
     * \brief Returns the device name.
     * \return QString
     */
    QString getDeviceName() const;

    /*!
     * \brief Returns the size of the status bar.
     * \return QSize
     */
    QSize getStatusBarSize() const;

    /*!
     * \brief Sets the status bar color.
     * \param color
     */
    void setStatusBarColor(const QColor &color);

    /*!
     * \brief Returns current status bar color.
     * \return QColor
     */
    QColor getStatusBarColor() const;

    /*!
     * \brief Opens the app's settings page.
     */
    void openAppSettings() const;

    /*!
     * \brief Calls the callback for image picker finished for the instance that opened it.
     * \param data
     * \return void
     */
    static void emitImagePickerFinished(QVariantMap data);

    /*!
     * \brief Calls the callback for image picker cancelled for the instance that opened it.
     * \return void
     */
    static void emitImagePickerCancelled();

    /*!
     * \brief Calls onKeyboardHeightChanged function on every valid instance.
     * \param height
     * \return void
     */
    static void emitKeyboardHeightChangedSignals(int height);

    /*!
     * \brief Calls onAlertDialogClicked for the instance that called the alert dialog.
     * \param index
     * \return void
     */
    static void emitAlertDialogClickedSignal(unsigned int index);

    /*!
     * \brief Calls onActionSheetClicked for the instance that called the alert dialog.
     * \param index
     * \return void
     */
    static void emitActionSheetDialogClickedSignal(unsigned int index);

private:

    /*!
     * \internal
     * \variable static QMap<int, CacheManager *> m_Instances
     * \brief This variable is used to keep track of all the instances so that we can send signals to all of them.
     */
    static QMap<int, iOSNativeUtils *> m_Instances;

    /*!
     * \variable static unsigned int m_NextInstanceID
     * \brief Used to prevent key clashes in m_Instances.
     */
    static unsigned int m_NextInstanceID;

    /*!
     * \internal
     * \variable const int m_InstanceIndex
     * \brief This variable is initilized in the constructor to the size value of m_Instances.
     *
     * This is used to identify the current instance in m_Instances.
     */
    const unsigned int m_InstanceIndex;

    bool m_IsImagePickerOpen,
         m_IsAlertDialogVisible,
         m_IsActionSheetDialogVisible,
         m_IsCameraOpen;

private:

    /*!
     * \internal
     * \brief Invokes onImagePickerControllerFinishedPicking If it is valid and If the image picker is open.
     * \param data
     */
    void callImagePickerFinishedCallback(QVariantMap &data);

    /*!
     * \internal
     * \brief Invokes onImagePickerControllerCancelled If it is valid and If the image picker is open.
     */
    void callImagePickerCancelledCallback();

    /*!
     * \internal
     * \brief Invokes onKeyboardHeightChanged If it is valid.
     * \param height
     */
    void emitKeyboardHeightChanged(int height);

    /*!
     * \internal
     * \brief Invokes onAlertDialogClicked if it is valid and If this instance opened the alert dialog.
     * \param index
     */
    void callAlertDialogClickedCallback(unsigned int index);

    /*!
     * \internal
     * \brief Invokes onActionSheetClicked if it is valid and If this instance opened the action sheet.
     * \param index
     */
    void callActionSheetDialogClickedCallback(unsigned int index);
};

}
