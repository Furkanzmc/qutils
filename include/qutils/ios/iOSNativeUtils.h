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
     * @brief Returns teh FCM token if it was generated. If it wasn't, returns an empty string.
     * @return QString
     */
    QString getFCMToken() const;
};
