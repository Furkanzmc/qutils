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
    ~iOSUtils();

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

    /**
     * @brief If Safari services is enabled, opens the link in an in app browser. Otherwise it does nothing.
     * @param url
     * @return void
     */
    Q_INVOKABLE void openSafari(const QString &url);

    /**
     * @brief See iOSNativeUtils::requestLocationPermission.
     * @return void
     */
    Q_INVOKABLE void requestLocationPermission();

    /**
     * @brief See iOSNativeUtils::getLocationAuthorizationStatus.
     * @return LocationAuthorizationStatus
     */
    int getLocationAuthorizationStatus();

    /**
     * @brief Only the first instance will be notified of this.
     * @param url
     */
    static void emitOpenedWithURLSignal(const QString &url);

    /**
     * @brief Only the first instance will be notified of this.
     * @param url
     */
    static void emitOpenedWithoutURLSignal();

signals:
    void alertDialogClicked(int buttonIndex);
    void actionSheetClicked(int buttonIndex);
    void keyboardHeightChanged(int keyboardHeight);

    /**
     * @brief This signal is emitted when the app is opened with a URL.
     * Follow this tutorial for how to implement int into your app: https://developer.android.com/training/app-links/index.html
     * @param url
     */
    void openedWithURL(const QString &url);

    /**
     * @brief This is also called for the first instance. This is just a complementary signal. This is emitted If an app is not opened from a URL.
     */
    void openedWithoutURL();

private:
    static QList<iOSUtils *> m_Instances;
    static QString m_URLOpenedWith;

    const int m_InstanceID;
    iOSNativeUtils *m_iOSNative;
};

}
