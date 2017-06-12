// Qt
#include <QStringList>
// std
#include <functional>

class iOSNativeUtils
{
public:
    std::function<void(unsigned int /*buttonIndex*/)> onAlertDialogClicked;

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
};
