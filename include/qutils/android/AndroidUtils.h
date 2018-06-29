#pragma once
// Qt
#include <QColor>
#include <QObject>
#include <QtAndroidExtras/QAndroidJniObject>
// Local
#include "qutils/NotificationClient.h"

namespace zmc
{

/**
 * @brief The AndroidButtonEvent class can be used to break the button event propagation to the other AndroidUtils classes.
 * If you set m_IsAccepted to true, the event propagation will be stopped.
 */
class AndroidButtonEvent : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool accepted READ isAccepted WRITE setAccepted NOTIFY acceptedChanged)

public:
    AndroidButtonEvent(QObject *parent = nullptr);

    bool isAccepted() const;
    void setAccepted(bool accepted);

signals:
    // This is just there to prevent warnings, it will not be emitted.
    void acceptedChanged();

private:
    bool m_IsAccepted;
};

class AndroidUtils : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool buttonEventsEnabled READ isButtonEventsEnabled WRITE setButtonEventsEnabled NOTIFY buttonEventsEnabledChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit AndroidUtils(QObject *parent = 0);
    ~AndroidUtils();

    Q_INVOKABLE void setStatusBarColor(QColor color);
    Q_INVOKABLE QString getStatusBarColor();
    Q_INVOKABLE void setStatusBarVisible(bool visible);

    Q_INVOKABLE bool isStatusBarVisible() const;
    Q_INVOKABLE void setImmersiveMode(bool visible);
    Q_INVOKABLE void shareText(const QString &dialogTitle, const QString &text);

    /**
     * @brief If m_IsEnabled is false, you cannot use this function.
     * @return void
     */
    Q_INVOKABLE void showDatePicker();

    /**
     * @brief If m_IsEnabled is false, you cannot use this function.
     * @return void
     */
    Q_INVOKABLE void showTimePicker();

    /**
     * @brief If m_IsEnabled is false, you cannot use this function.
     * @return void
     */
    Q_INVOKABLE void showCamera(const QString &fileName);

    /**
     * @brief Shows toast with the given text.
     * @return void
     */
    Q_INVOKABLE void showToast(const QString &text, bool isLongDuration);

    /**
     * @brief Opens the gallery for an pick file operation. If m_IsEnabled is false, you cannot use this function.
     * The MIME type defaults to image/\*.
     * @return void
     */
    Q_INVOKABLE void openGallery();

    /**
     * @brief Opens the gallery for an pick file operation. If m_IsEnabled is false, you cannot use this function.
     * You can split the MIME types with "|" to set multiple MIME types. Example: `application/pdf|image/jpeg|text/plain`.
     * @param fileType The MIME type for the type of file to be selected.
     * @return void
     */
    Q_INVOKABLE void openDocumentPicker(const QString &fileType);

    /**
     * @brief Dismisses the keyboard from the view.
     * @return void
     */
    Q_INVOKABLE void dismissKeyboard();

    /**
     * @brief Returns the device model
     * @return QString
     */
    Q_INVOKABLE QString getDeviceModel();

    bool isButtonEventsEnabled() const;
    void setButtonEventsEnabled(bool enabled);

    /**
     * @brief Returns the enabled property.
     * @return bool
     */
    bool isEnabled() const;

    /**
     * @brief If m_IsEnabled is set to false, this instance will not receive any signals from the system. So, when m_IsEnabled is false, you cannot show an
     * alert dialog or show the date picker. Basically, anything that requires a callback back to you cannot be used.
     * The only exceptions are the openedWithoutURL() and openedWithURL() signals.
     * @param enabled
     * @return void
     */
    void setEnabled(bool enabled);

    /**
     * @brief Signals are not emitted if the application state is not Qt::ApplicationActive. AndroidUtils starts from the latest created AndroidUtils instance
     * and works its way down to the bottom. And every instance who has the buttonEventsEnabled property true is notified of the signal. The default
     * value of the property is false.
     * @param isBackButton
     * @param isMenuButton
     */
    static void emitButtonPressedSignals(bool isBackButton, bool isMenuButton);
    static void emitDatePickedSignals(int year, int month, int day);

    static void emitTimePickedSignals(int hourOfDay, int minute);
    static void emitCameraCapturedSignals(const QString &capturePath);
    static void emitFileSelectedSignals(const QString &filePath);

    static void emitKeyboardHeightChangedSignals(const int &keyboardHeight);
    static void emitCameraCaptureCancelledSignals();
    static void emitFileSelectionCancelledSignals();

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

    /**
     * @brief Returns true if this is the main controller.
     * @return bool
     */
    Q_INVOKABLE bool isMainController() const;

    /**
     * @brief Set the main controller to this one. When an instance is set as main controller, certain signals will only be emitted for this instance.
     * Only one instance can be the main controller. When an instance is set as main controller while there is already an existing main controller, the existing
     * main controller will relinquish its position and the new instance will be the new main controller.
     * The signals that are designated to main controller are:
     * - openedWithUrl
     * - openedWithoutUrl
     * @param isMain
     * @return void
     */
    Q_INVOKABLE void setMainController(bool isMain, bool disableOthers = true);

signals:
    /**
     * @brief This signaled everytime the back button is pressed. For now, this behaviour overrides the close behaviour of the Window. So you need to manually
     * close it yourself. Only the last instance is informed of the back button signal.
     */
    void backButtonPressed(AndroidButtonEvent *event);

    /**
     * @brief Only the last instance is informed of the menu button signal.
     */
    void menuButtonPressed(AndroidButtonEvent *event);

    void datePicked(int year, int month, int day);
    void datePickerCancelled();
    void timePicked(int hourOfDay, int minute);

    void timePickerCancelled();
    void cameraCaptured(const QString &capturePath);
    void cameraCaptureCancelled();

    void fileSelected(const QStringList &filePaths);
    void fileSelectionCancelled();
    void photoSelected(const QString &filePath);
    void photoSelectionCanceled();

    void keyboardHeightChanged(int keyboardHeight);
    void buttonEventsEnabledChanged();
    void enabledChanged();

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

    /**
     * @brief Emitted when m_IsMainController is changed.
     * @return void
     */
    void mainControllerChanged();

private:
    /*!
     * \variable static QMap<int, CacheManager *> m_Instances
     * \brief This variable is used to keep track of all the instances so that we can send signals to all of them.
     */
    static std::map<int, AndroidUtils *> m_Instances;
    static QString m_URLOpenedWith;

    /*!
     * \variable const int m_InstanceID
     * \brief This variable is initilized in the constructor to the size value of m_Instances.
     *
     * This is used to identify the current instance in m_Instances.
     */
    const int m_InstanceID;
    bool m_IsAlertShown,
         m_IsDatePickerShown,
         m_IsTimePickerShown,
         m_IsCameraShown,
         m_IsGalleryShown,
         m_IsDocumentPickerShown,
         m_IsMainController;
    /**
     * @brief The default value is false. This is used to determine which instance to send the backButtonPressed signal.
     */
    bool m_IsButtonEventsEnabled;

private:
    void emitBackButtonPressed(AndroidButtonEvent *event);
    void emitMenuButtonPressed(AndroidButtonEvent *event);

    void emitDatePicked(int year, int month, int day);
    void emitTimePicked(int hourOfDay, int minute);
    void emitCameraCaptured(const QString &capturePath);

    void emitCameraCaptureCancelled();
    void emitFileSelected(QString filePath);
    void emitFileSelectionCancelled();

    void emitKeyboardHeightChanged(const int &keyboardHeight);

    /**
     * @brief Converts a QVariantMap to HashMap in Java. Supported types are:
     * - Integer
     * - Bool
     * - QVariantMap
     * - List (Sadly, only String lists for now)
     * @param map
     * @return QAndroidJniObject
     */
    QAndroidJniObject getJNIHashMap(const QVariantMap &map) const;
};

}
