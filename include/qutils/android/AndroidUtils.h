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
     * // Show an alert sheet with items
     * var properties = {
     *     "title": "Select An Item",
     *     "items": [
     *         "Item 1",
     *         "Item 2"
     *     ]
     * };
     *
     * nu.showAlertDialog(properties);
     *
     * // Show an alert sheet with three buttons and a message
     * var properties = {
     *     "positive": "Yes",
     *     "negative": "No",
     *     "neutral": "Maybe",
     *     "title": "Would you?",
     *     "message": "Would you not?"
     * };
     *
     * nu.showAlertDialog(properties);
     * @endcode
     * @param dialogData
     */
    Q_INVOKABLE void showAlertDialog(const QVariantMap &dialogProperties);

    Q_INVOKABLE void showDatePicker();
    Q_INVOKABLE void showTimePicker();
    Q_INVOKABLE void showCamera(const QString &filePath);

    static void emitButtonPressedSignals(bool isBackButton, bool isMenuButton);
    static void emitAlertDialogClickedSignals(int buttonType, int itemIndex);
    static void emitDatePickedSignals(int year, int month, int day);

    static void emitTimePickedSignals(int hourOfDay, int minute);

signals:
    /**
     * @brief This signaled everytime the back button is pressed. For now, this behaviour overrides the close behaviour of the Window. So you need to manually
     * close it yourself.
     */
    void backButtonPressed();
    void menuButtonPressed();
    void alertDialogClicked(int buttonType);

    void alertDialogCancelled();
    void datePicked(int year, int month, int day);
    void datePickerCancelled();

    void timePicked(int hourOfDay, int minute);
    void timePickerCancelled();
    void alertDialogItemClicked(int itemIndex);

private:
    static std::vector<AndroidUtils *> m_Instances;
    static int m_LastInstanceID;

    int m_InstanceID;
    bool m_IsAlertShown,
         m_IsDatePickerShown,
         m_IsTimePickerShown;

private:
    void emitBackButtonPressed();
    void emitMenuButtonPressed();
    void emitAlertDialogClicked(int buttonType, int itemIndex);

    void emitDatePicked(int year, int month, int day);
    void emitTimePicked(int hourOfDay, int minute);

    /**
     * @brief Converts a QVariantMap to HashMap in Java. Supported types are:
     * - Integer
     * - Bool
     * - QVariantMap
     * - List (Sadly, only String lists for now)
     * @param map
     * @return
     */
    QAndroidJniObject getJNIHashMap(const QVariantMap &map) const;
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

static void alertDialogClickedCallback(JNIEnv */*env*/, jobject /*obj*/, jint buttonType, jint itemIndex)
{
    zmc::AndroidUtils::emitAlertDialogClickedSignals(buttonType, itemIndex);
}

static void datePickedCallback(JNIEnv */*env*/, jobject /*obj*/, jint year, jint month, jint day)
{
    zmc::AndroidUtils::emitDatePickedSignals(year, month, day);
}

static void timePickedCallback(JNIEnv */*env*/, jobject /*obj*/, jint hourOfDay, jint minute)
{
    zmc::AndroidUtils::emitTimePickedSignals(hourOfDay, minute);
}
