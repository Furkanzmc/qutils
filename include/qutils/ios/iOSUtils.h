#pragma once
#include <QObject>
#include <QVariantMap>

class iOSNativeUtils;

namespace zmc
{

class iOSUtils : public QObject
{
    Q_OBJECT

public:
    explicit iOSUtils(QObject *parent = nullptr);

    /**
     * @brief Shows a native AlertDialog according to the given dialog properties.
     * You can mix the properties for both Android and iOS. Only the related properties will be used by this function.
     *
     * **Example**
     * @code
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
     * @endcode
     * @param dialogData
     */
    Q_INVOKABLE void showAlertView(const QVariantMap &dialogProperties);

    /**
     * @brief Show the default share dialog on iOS. There's no dialog title on iOS.
     * @param text
     */
    Q_INVOKABLE void shareText(const QString &text);

    Q_INVOKABLE void showActionSheet(const QString &title, const QString &message, const QVariantList &buttons);

    /**
     * @brief Dismisses the keyboard.
     * @return void
     */
    Q_INVOKABLE void dismissKeyboard();

    /**
     * @brief Sets the application's icon badge number. Set to 0 to clear it.
     * @param number
     * @return void
     */
    Q_INVOKABLE void setApplicationIconBadgeNumber(const int &number);

    /**
     * @brief Returns true If the device running the app is an iPad.
     * @return bool
     */
    Q_INVOKABLE bool isiPad() const;

signals:
    void alertDialogClicked(int buttonIndex);
    void actionSheetClicked(int buttonIndex);
    void keyboardHeightChanged(int keyboardHeight);

private:
    iOSNativeUtils *m_iOSNative;
};

}
