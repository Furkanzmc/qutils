#pragma once
#include <QObject>
#include <QColor>
// qutils
#include "qutils/Macros.h"
#ifdef Q_OS_ANDROID
// qutils
#include "qutils/android/AndroidUtils.h"
#endif // Q_OS_ANDRID
#ifdef Q_OS_IOS
#include "qutils/ios/iOSUtils.h"
#endif // Q_OS_IOS

namespace zmc
{

/**
 * @brief NativeUtils class is used as a main source of entry to both Android and iOS features. When a feature is not supported on the current development
 * platform, the feature will be disabled but it will also not cause a compilation error.
 */
class NativeUtils : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool buttonEventsEnabled READ isButtonEventsEnabled WRITE setButtonEventsEnabled NOTIFY buttonEventsEnabledChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit NativeUtils(QObject *parent = 0);

    Q_INVOKABLE void setStatusBarColor(QColor color);
    Q_INVOKABLE void setStatusBarVisible(bool visible);
    Q_INVOKABLE void setImmersiveMode(bool visible);

    Q_INVOKABLE void shareText(const QString &dialogTitle, const QString &text);
    Q_INVOKABLE void showAlertDialog(const QVariantMap &dialogProperties);
    Q_INVOKABLE void showDatePicker();

    Q_INVOKABLE void showTimePicker();
    Q_INVOKABLE void showCamera(const QString &photoName);
    Q_INVOKABLE void showToast(const QString &text, bool isLongDuration);

    Q_INVOKABLE void showActionSheet(const QString &title, const QString &message, const QVariantList &buttons);
    Q_INVOKABLE void openGallery();

    bool isButtonEventsEnabled() const;
    void setButtonEventsEnabled(bool enabled);

    bool isEnabled() const;
    void setEnabled(bool enabled);

signals:
    void backButtonPressed();
    void menuButtonPressed();
    void alertDialogClicked(int buttonIndex);

    void datePicked(int year, int month, int day);
    void datePickerCancelled();
    void timePicked(int hourOfDay, int minute);

    void timePickerCancelled();
    void cameraCaptured(const QString &capturePath);
    void cameraCaptureCancelled();

    void actionSheetClicked(int buttonIndex);
    void fileSelected(const QString &filePath);
    void fileSelectionCancelled();

    void keyboardHeightChanged(int keyboardHeight);
    void buttonEventsEnabledChanged();
    void enabledChanged();

    void openedWithURL(const QString &url);
    void openedWithoutURL();

private:
#ifdef Q_OS_ANDROID
    AndroidUtils *m_AndroidUtils;
#endif // Q_OS_ANDROID
#ifdef Q_OS_IOS
    static iOSUtils *m_iOSUtils;
#endif // Q_OS_IOS
};

}
