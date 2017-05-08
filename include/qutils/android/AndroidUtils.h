#pragma once
// Qt
#include <QObject>
#include <QColor>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#endif // Q_OS_ANDROID
// Local
#include "qutils/android/NotificationClient_Android.h"

namespace zmc
{

class AndroidUtils : public QObject
{
    Q_OBJECT
public:
    explicit AndroidUtils(QObject *parent = 0);
    ~AndroidUtils();

    Q_INVOKABLE void setStatusBarColor(QColor color);
    Q_INVOKABLE void setStatusBarVisible(bool visible);
    Q_INVOKABLE void setImmersiveMode(bool visible);

    Q_INVOKABLE void shareText(const QString &dialogTitle, const QString &text);

    /**
     * @brief Shows a native AlertDialog according to the given dialog properties.
     * `title` and `message` properties are mandatory. And at least one type of button should be given.
     * When it is clicked, these values are returned with the signal:
     * - Positive Button: 1
     * - Neutral Button: 0
     * - Negative Button: -1
     *
     * **Example**
     * @code
     * var properties = {
     *     "No": {
     *         "type": "negative"
     *     },
     *     "Not Sure": {
     *         "type": "neutral"
     *     },
     *     "Sure": {
     *         "type": "positive"
     *     },
     *     "title": "Here's a dialog.",
     *     "message": "Would you like something?"
     * };
     *
     * nativeUtils.showAlertDialog(properties);
     * @endcode
     * @param dialogData
     */
    Q_INVOKABLE void showAlertDialog(const QVariantMap &dialogProperties);

    static void emitButtonPressedSignals(bool isBackButton, bool isMenuButton);
    static void emitAlertDialogClickedSignals(int buttonType);

signals:
    /**
     * @brief This signaled everytime the back button is pressed. For now, this behaviour overrides the close behaviour of the Window. So you need to manually
     * close it yourself.
     */
    void backButtonPressed();
    void menuButtonPressed();
    void alertDialogClicked(int buttonType);

    void alertDialogCancelled();

private:
    static std::vector<AndroidUtils *> m_Instances;
    static int m_LastInstanceID;

    int m_InstanceID;
    bool m_IsAlertShown;

private:
    void emitBackButtonPressed();
    void emitMenuButtonPressed();
    void emitAlertDialogClicked(int buttonType);

    /**
     * @brief Converts a QVariantMap to HashMap in Java.
     * @param map
     * @return
     */
    QAndroidJniObject getHashMap(const QVariantMap &map) const;
};

}

static void notificationReceivedCallback(JNIEnv */*env*/, jobject /*obj*/, jstring jtag, jint id, jstring jnotificationManagerName)
{
    const QString tag = QAndroidJniObject(jtag).toString();
    const QString managerName = QAndroidJniObject(jnotificationManagerName).toString();
    zmc::NotificationClient *client = zmc::NotificationClient::getInstance(tag, id);
    if (client) {
        client->emitNotificationReceivedSignal(tag, id);
    }
    else {
        zmc::NotificationClient::addNotifiationQueue(std::make_tuple(tag, id, managerName));
    }
}

static void backButtonPressedCallback(JNIEnv */*env*/, jobject /*obj*/)
{
    zmc::AndroidUtils::emitButtonPressedSignals(true, false);
}

static void menuButtonPressedCallback(JNIEnv */*env*/, jobject /*obj*/)
{
    zmc::AndroidUtils::emitButtonPressedSignals(false, true);
}

static void alertDialogClickedCallback(JNIEnv */*env*/, jobject /*obj*/, jint buttonType)
{
    zmc::AndroidUtils::emitAlertDialogClickedSignals(buttonType);
}
