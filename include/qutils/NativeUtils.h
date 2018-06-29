#pragma once
// Qt
#include <QRect>
#include <QColor>
#include <QObject>
// Local
#include "qutils/Macros.h"
#ifdef Q_OS_ANDROID
    #include "qutils/android/AndroidUtils.h"
#endif // Q_OS_ANDRID

namespace zmc
{

#ifdef Q_OS_IOS
    class iOSUtils;
#endif // Q_OS_IOS
#if defined(Q_OS_MACOS) && !defined(Q_OS_IOS)
    class MacOSUtils;
#endif // Q_OS_MACOS

/**
 * @brief NativeUtils class is used as a main source of entry to both Android and iOS features. When a feature is not supported on the current development
 * platform, the feature will be disabled but it will also not cause a compilation error.
 */
class NativeUtils : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool buttonEventsEnabled READ isButtonEventsEnabled WRITE setButtonEventsEnabled NOTIFY buttonEventsEnabledChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool ipad READ isiPad CONSTANT)

    Q_PROPERTY(bool mainController READ isMainController WRITE setMainController NOTIFY mainControllerChanged)

public:
    explicit NativeUtils(QObject *parent = 0);

    Q_INVOKABLE void setStatusBarColor(QColor color);
    Q_INVOKABLE QString getStatusBarColor();
    Q_INVOKABLE void setStatusBarVisible(bool visible);

    Q_INVOKABLE void setImmersiveMode(bool visible);
    Q_INVOKABLE void shareText(const QString &dialogTitle, const QString &text);
    Q_INVOKABLE void showAlertDialog(const QVariantMap &dialogProperties);

    Q_INVOKABLE void showTimePicker();
    Q_INVOKABLE void showCamera(const QString &photoName);

    Q_INVOKABLE void showToast(const QString &text, bool isLongDuration);
    Q_INVOKABLE void showActionSheet(const QString &title, const QString &message, const QVariantList &buttons, QRect rect = QRect());
    Q_INVOKABLE void dismissKeyboard();

    Q_INVOKABLE void setApplicationIconBadgeNumber(const unsigned int &number);
    Q_INVOKABLE bool isiPad() const;
    Q_INVOKABLE void openSafari(const QString &url);

    Q_INVOKABLE QString getDeviceModel();
    Q_INVOKABLE bool isStatusBarVisible() const;
    Q_INVOKABLE QSize getStatusBarSize() const;
    Q_INVOKABLE void openAppSettings() const;

    bool isButtonEventsEnabled() const;
    void setButtonEventsEnabled(bool enabled);
    bool isEnabled() const;

    void setEnabled(bool enabled);
    bool isMainController() const;
    void setMainController(bool isMain);

signals:
#ifdef Q_OS_ANDROID
    void backButtonPressed(AndroidButtonEvent *event);
    void menuButtonPressed(AndroidButtonEvent *event);
#else
    void backButtonPressed(void *event);
    void menuButtonPressed(void *event);
#endif // Q_OS_ANDRID
    void alertDialogClicked(int buttonIndex);

    void timePicked(int hourOfDay, int minute);

    void timePickerCancelled();
    void cameraCaptured(const QString &capturePath);
    void cameraCaptureCancelled();

    void actionSheetClicked(int buttonIndex);
    void keyboardHeightChanged(int keyboardHeight);
    void buttonEventsEnabledChanged();

    void enabledChanged();
    void openedWithURL(const QString &url);
    void openedWithoutURL();

    void mainControllerChanged();

private:
#if defined(Q_OS_ANDROID)
    AndroidUtils *m_AndroidUtils;
#elif defined(Q_OS_IOS)
    iOSUtils *m_iOSUtils;
#elif defined(Q_OS_MACOS) && !defined(Q_OS_IOS)
    MacOSUtils *m_MacOSUtils;
#endif // Platform Check
};

}
