// Qt
#include <QStringList>
#include <QVariantList>
// std
#include <functional>

namespace zmc
{

class iOSNativeUtils
{
public:
    std::function<void(unsigned int /*buttonIndex*/)> onAlertDialogClicked;
    std::function<void(unsigned int /*buttonIndex*/)> onActionSheetClicked;
    std::function<void()> onImagePickerControllerCancelled;
    std::function<void()> onCameraCancelled;

    /**
     * @brief The following keys will be present in the map:
     * - mediaType: The media type for the file.
     * - tempUrl: The file that resides at the temporary location for read and write access by the app.
     * - referenceUrl: The reference URL provided by PHImageManager.
     * - isSourceCamera: A bool value that indicates If the image was taken from the camera.
     */
    std::function<void(const QVariantMap &/*info*/)> onImagePickerControllerFinishedPicking;

    /**
     * @brief If the keyboard is hidden then the width and height will be zero. Otherwise they will be non-zero.
     * Keyboard's size will be reported as zero with UIKeyboardWillHideNotification. And the keyboard height will change with UIKeyboardWillShowNotification.
     */
    std::function<void(int /*height*/)> onKeyboardHeightChanged;

    /**
     * @brief This is called when the user authenticates the app to access to the photos.
     */
    std::function<void()> onPhotosAccessGranted;

    /**
     * @brief This is called when the user declines authentication to access to the photos.
     */
    std::function<void()> onPhotosAccessDenied;

    enum LocationAuthorizationStatus {
        LANone = 0, // The location services is not in use.
        LANotDetermined = 1, // The user has not yet made a choice regarding whether this app can use location services.
        LARestricted = 2, // This app is not authorized to use location services.
        LADenied = 3, // The user explicitly denied the use of location services for this app or location services are currently disabled in Settings.
        LAAuthorizedAlways = 4, // This app is authorized to start location services at any time.
        LAAuthorizedWhenInUse = 5 // This app is authorized to start most location services while running in the foreground.
    };

    enum PhotosAuthorizationStatus {
        PANone = 0, // Photos is not enabled for quitls.
        PANotDetermined = 1, // Explicit user permission is required for photo library access, but the user has not yet granted or denied such permission.
        PARestricted = 2, // Your app is not authorized to access the photo library, and the user cannot grant such permission.
        PADenied = 3, // The user has explicitly denied your app access to the photo library.
        PAAuthorized = 4 // The user has explicitly granted your app access to the photo library.
    };

public:
    iOSNativeUtils();
    ~iOSNativeUtils();

    /**
     * @brief Shows a native alert view on iOS. Make sure that you connect the onAlertDialogClicked lamda to get a callback.
     * @param title
     * @param message
     * @param buttons
     */
    void showAlertView(const QString &title, const QString &message, const QStringList &buttons);

    /**
     * @brief Share text with the default share dialog.
     * @param text
     */
    void shareText(const QString &text);

    /**
     * @brief buttons format is as follows.
     * [
     *     {
     *         "title": "Button 1",
     *         "type": "cancel"
     *     }
     * ]
     * Button type options are: "cancel", "destructive" and "default". If anything other than these are given, "default" is used.
     * @param title
     * @param message
     * @param buttons
     */
    void showActionSheet(const QString &title, const QString &message, const QVariantList &buttons);

    void schedulePushNotification(const QString &title, const QString &body, const int &delayInSeconds);

    /**
     * @brief Hides the keyboard.
     * @return void
     */
    void dismissKeyboard();

    /**
     * @brief Returns teh FCM token if it was generated. If it wasn't, returns an empty string.
     * @return QString
     */
    QString getFCMToken() const;

    /**
     * @brief Sets the application's icon badge number. Set to 0 to clear it.
     * @param number
     * @return void
     */
    void setApplicationIconBadgeNumber(const int &number);

    /**
     * @brief Returns true If the device running the app is an iPad.
     * @return bool
     */
    bool isiPad() const;

    /**
     * @brief Uses iOS SafariServices to open the url in an in-app browser. The browser events are not yet supported.
     * This function requires safari_services in QUTILS_FEATURES.
     * @param url
     * @return void
     */
    void openSafari(const QString &url);

    /**
     * @brief Requests the location services permission.
     * @return void
     */
    void requestLocationPermission();

    /**
     * @brief Returns the current location authorization status from the device. If location is not enabled for qutils, LocationAuthorizationStatus::None is
     * returned.
     * @return LocationAuthorizationStatus
     */
    LocationAuthorizationStatus getLocationAuthorizationStatus();

    /**
     * @brief Requests access to photos on iOS.
     * @return void
     */
    void requestPhotosPermisson();

    /**
     * @brief Returns the current photos access authorization status from the device. If photos is not enabled for qutils, PhotosAuthorizationStatus::None is
     * returned.
     * @return LocationAuthorizationStatus
     */
    PhotosAuthorizationStatus getPhotosAuthorizationStatus();

    /**
     * @brief Opens the gallery to pick an image.
     * @return void
     */
    void openGallery();

    /**
     * @brief Opens the camera.
     * @return void
     */
    void showCamera();

    /**
     * @brief Hide or show the status bar.
     * In order for this to take effect you need to have `UIViewControllerBasedStatusBarAppearance` set to false in your Info.plist file.
     * @param visible
     * @return void
     */
    void setStatusBarVisible(bool visible);

    /**
     * @brief Calls the callback for image picker finished for the instance that opened it.
     * @param data
     * @return void
     */
    static void emitImagePickerFinished(QVariantMap data);

    /**
     * @brief Calls the callback for image picker cancelled for the instance that opened it.
     * @return void
     */
    static void emitImagePickerCancelled();

    /**
     * @brief This will be set to true when the image picker is open and only this instance will be notified of the events from the image picker.
     * @return bool
     */
    bool isImagePickerOpen() const;

    /**
     * @brief This will be true when this instance calls the camera.
     * @return bool
     */
    bool isCameraOpen() const;

    /**
     * @brief Returns true If the status bar is hidden.
     * @return bool
     */
    bool isStatusBarVisible() const;

    /**
     * @brief Calls onKeyboardHeightChanged function on every valid instance.
     * @param height
     * @return void
     */
    static void emitKeyboardHeightChangedSignals(int height);

    /**
     * @brief Calls onAlertDialogClicked for the instance that called the alert dialog.
     * @param index
     * @return void
     */
    static void emitAlertDialogClickedSignal(unsigned int index);

    /**
     * @brief Calls onActionSheetClicked for the instance that called the alert dialog.
     * @param index
     * @return void
     */
    static void emitActionSheetDialogClickedSignal(unsigned int index);

    /**
     * @brief Calls the appropriate permission callback for the caller instance.
     * @param isAccessGranted
     */
    static void emitPhotoAccessPermissionSignals(bool isAccessGranted);

private:
    static QList<iOSNativeUtils *> m_Instances;
    const unsigned int m_InstanceIndex;
    bool m_IsImagePickerOpen,
         m_IsAlertDialogVisible,
         m_IsActionSheetDialogVisible,
         m_IsPhotoAccessPermissionRequested,
         m_IsCameraOpen;

private:
    void callImagePickerFinishedCallback(QVariantMap &data);
    void callImagePickerCancelledCallback();
    void emitKeyboardHeightChanged(int height);

    void callAlertDialogClickedCallback(unsigned int index);
    void callActionSheetDialogClickedCallback(unsigned int index);
    void callPhotoAccessResultCallback(bool isAccessGranted);
};

}
