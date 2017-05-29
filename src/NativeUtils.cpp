#include "qutils/NativeUtils.h"
// Qt
#include <QStandardPaths>

namespace zmc
{

#ifdef Q_OS_ANDROID
AndroidUtils *NativeUtils::m_AndroidUtils = new AndroidUtils();
#endif // Q_OS_ANDROID

NativeUtils::NativeUtils(QObject *parent)
    : QObject(parent)
{
#ifdef Q_OS_ANDROID
    connect(m_AndroidUtils, &AndroidUtils::backButtonPressed, this, &NativeUtils::backButtonPressed);
    connect(m_AndroidUtils, &AndroidUtils::menuButtonPressed, this, &NativeUtils::menuButtonPressed);
    connect(m_AndroidUtils, &AndroidUtils::alertDialogClicked, this, &NativeUtils::alertDialogClicked);

    connect(m_AndroidUtils, &AndroidUtils::alertDialogCancelled, this, &NativeUtils::alertDialogCancelled);
    connect(m_AndroidUtils, &AndroidUtils::alertDialogItemClicked, this, &NativeUtils::alertDialogItemClicked);
    connect(m_AndroidUtils, &AndroidUtils::datePicked, this, &NativeUtils::datePicked);

    connect(m_AndroidUtils, &AndroidUtils::datePickerCancelled, this, &NativeUtils::datePickerCancelled);
    connect(m_AndroidUtils, &AndroidUtils::timePicked, this, &NativeUtils::timePicked);
    connect(m_AndroidUtils, &AndroidUtils::timePickerCancelled, this, &NativeUtils::timePickerCancelled);

    connect(m_AndroidUtils, &AndroidUtils::cameraCaptured, this, &NativeUtils::cameraCaptured);
#endif // Q_OS_ANDROID
}

void NativeUtils::setStatusBarColor(QColor color)
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->setStatusBarColor(color);
#else
    Q_UNUSED(color);
#endif // Q_OS_ANDROID
}

void NativeUtils::setStatusBarVisible(bool visible)
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->setStatusBarVisible(visible);
#else
    Q_UNUSED(visible);
#endif // Q_OS_ANDROID
}

void NativeUtils::setImmersiveMode(bool visible)
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->setImmersiveMode(visible);
#else
    Q_UNUSED(visible);
#endif // Q_OS_ANDROID
}

void NativeUtils::shareText(const QString &dialogTitle, const QString &text)
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->shareText(dialogTitle, text);
#else
    Q_UNUSED(dialogTitle);
    Q_UNUSED(text);
#endif // Q_OS_ANDROID
}

void NativeUtils::showAlertDialog(const QVariantMap &dialogProperties)
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->showAlertDialog(dialogProperties);
#else
    Q_UNUSED(dialogProperties);
#endif // Q_OS_ANDROID
}

void NativeUtils::showDatePicker()
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->showDatePicker();
#endif // Q_OS_ANDROID
}

void NativeUtils::showTimePicker()
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->showTimePicker();
#endif // Q_OS_ANDROID
}

void NativeUtils::showCamera(const QString &photoName)
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->showCamera(photoName);
#else
    Q_UNUSED(filePath);
#endif // Q_OS_ANDROID
}

}
