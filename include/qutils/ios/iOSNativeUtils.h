#pragma once
// Qt
#include <QRect>
#include <QColor>
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
     *
     * Button type options are: "cancel", "destructive" and "default". If anything other than these are given, "default" is used.
     * \a rect paremeter is required on iPads.
     * @param title
     * @param message
     * @param buttons
     * @param rect
     */
    void showActionSheet(const QString &title, const QString &message, const QVariantList &buttons, QRect rect = QRect());

    /*!
     * \brief This method DOES NOT work yet.
     * \param title
     * \param body
     * \param delayInSeconds
     */
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
     * @brief Hide or show the status bar.
     * In order for this to take effect you need to have `UIViewControllerBasedStatusBarAppearance` set to false in your Info.plist file.
     * @param visible
     * @return void
     */
    void setStatusBarVisible(bool visible);

    /**
     * @brief Returns true If the status bar is hidden.
     * @return bool
     */
    bool isStatusBarVisible() const;

    /**
     * @brief Returns the device name.
     * @return QString
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

private:

    /*!
     * \variable static QMap<int, CacheManager *> m_Instances
     * \brief This variable is used to keep track of all the instances so that we can send signals to all of them.
     */
    static QMap<int, iOSNativeUtils *> m_Instances;

    /*!
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
    void callImagePickerFinishedCallback(QVariantMap &data);
    void callImagePickerCancelledCallback();
    void emitKeyboardHeightChanged(int height);

    void callAlertDialogClickedCallback(unsigned int index);
    void callActionSheetDialogClickedCallback(unsigned int index);
};

}
