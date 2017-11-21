// Qt
#include <QStringList>
#include <QVariantList>
// std
#include <functional>

class iOSNativeUtils
{
public:
    std::function<void(unsigned int /*buttonIndex*/)> onAlertDialogClicked;
    std::function<void(unsigned int /*buttonIndex*/)> onActionSheetClicked;

    /**
     * @brief If the keyboard is hidden then the width and height will be zero. Otherwise they will be non-zero.
     * Keyboard's size will be reported as zero with UIKeyboardWillHideNotification. And the keyboard height will change with UIKeyboardWillShowNotification.
     */
    std::function<void(int /*height*/)> onKeyboardHeightChanged;

    enum LocationAuthorizationStatus {
        None = 0, // The location services is not in use.
        NotDetermined = 1, // The user has not yet made a choice regarding whether this app can use location services.
        Restricted = 2, // This app is not authorized to use location services.
        Denied = 3, // The user explicitly denied the use of location services for this app or location services are currently disabled in Settings.
        AuthorizedAlways = 4, // This app is authorized to start location services at any time.
        AuthorizedWhenInUse = 5 // This app is authorized to start most location services while running in the foreground.
    };

public:
    iOSNativeUtils();

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
};
