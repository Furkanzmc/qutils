#include "qutils/NativeUtils.h"
// Qt
#include <QStandardPaths>

namespace zmc
{

NativeUtils::NativeUtils(QObject *parent)
    : QObject(parent)
#ifdef Q_OS_ANDROID
    , m_AndroidUtils(new AndroidUtils(this))
#endif // Q_OS_ANDROID
#ifdef Q_OS_IOS
    , m_iOSUtils(new iOSUtils())
#endif // Q_OS_IOS
{
#ifdef Q_OS_ANDROID
    connect(m_AndroidUtils, &AndroidUtils::backButtonPressed, this, &NativeUtils::backButtonPressed);
    connect(m_AndroidUtils, &AndroidUtils::menuButtonPressed, this, &NativeUtils::menuButtonPressed);
    connect(m_AndroidUtils, &AndroidUtils::alertDialogClicked, this, &NativeUtils::alertDialogClicked);

    connect(m_AndroidUtils, &AndroidUtils::datePicked, this, &NativeUtils::datePicked);
    connect(m_AndroidUtils, &AndroidUtils::datePickerCancelled, this, &NativeUtils::datePickerCancelled);
    connect(m_AndroidUtils, &AndroidUtils::timePicked, this, &NativeUtils::timePicked);

    connect(m_AndroidUtils, &AndroidUtils::timePickerCancelled, this, &NativeUtils::timePickerCancelled);
    connect(m_AndroidUtils, &AndroidUtils::cameraCaptured, this, &NativeUtils::cameraCaptured);
    connect(m_AndroidUtils, &AndroidUtils::cameraCaptureCancelled, this, &NativeUtils::cameraCaptureCancelled);

    connect(m_AndroidUtils, &AndroidUtils::fileSelected, this, &NativeUtils::fileSelected);
    connect(m_AndroidUtils, &AndroidUtils::fileSelectionCancelled, this, &NativeUtils::fileSelectionCancelled);
    connect(m_AndroidUtils, &AndroidUtils::keyboardHeightChanged, this, &NativeUtils::keyboardHeightChanged);

    connect(m_AndroidUtils, &AndroidUtils::buttonEventsEnabledChanged, this, &NativeUtils::buttonEventsEnabledChanged);
    connect(m_AndroidUtils, &AndroidUtils::enabledChanged, this, &NativeUtils::enabledChanged);
    connect(m_AndroidUtils, &AndroidUtils::openedWithURL, this, &NativeUtils::openedWithURL);

    connect(m_AndroidUtils, &AndroidUtils::openedWithoutURL, this, &NativeUtils::openedWithoutURL);
#endif // Q_OS_ANDROID

#ifdef Q_OS_IOS
    connect(m_iOSUtils, &iOSUtils::alertDialogClicked, this, &NativeUtils::alertDialogClicked);
    connect(m_iOSUtils, &iOSUtils::actionSheetClicked, this, &NativeUtils::actionSheetClicked);
    connect(m_iOSUtils, &iOSUtils::keyboardHeightChanged, this, &NativeUtils::keyboardHeightChanged);
#endif // Q_OS_IOS
}

void NativeUtils::setStatusBarColor(QColor color)
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->setStatusBarColor(color);
#else
    Q_UNUSED(color);
#endif // Q_OS_ANDROID
}

QString NativeUtils::getStatusBarColor()
{
    QString color = "black";
#ifdef Q_OS_ANDROID
    color = m_AndroidUtils->getStatusBarColor();
#endif // Q_OS_ANDROID

    return color;
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
#if defined(Q_OS_ANDROID)
    m_AndroidUtils->shareText(dialogTitle, text);
#elif defined (Q_OS_IOS)
    Q_UNUSED(dialogTitle);
    m_iOSUtils->shareText(text);
#else
    Q_UNUSED(dialogTitle);
    Q_UNUSED(text);
#endif // Q_OS_ANDROID
}

void NativeUtils::showAlertDialog(const QVariantMap &dialogProperties)
{
#if defined(Q_OS_ANDROID)
    m_AndroidUtils->showAlertDialog(dialogProperties);
#elif defined(Q_OS_IOS)
    m_iOSUtils->showAlertView(dialogProperties);
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
    Q_UNUSED(photoName);
#endif // Q_OS_ANDROID
}

void NativeUtils::showToast(const QString &text, bool isLongDuration)
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->showToast(text, isLongDuration);
#else
    Q_UNUSED(text);
    Q_UNUSED(isLongDuration);
#endif // Q_OS_ANDROID
}

void NativeUtils::showActionSheet(const QString &title, const QString &message, const QVariantList &buttons)
{
#ifdef Q_OS_IOS
    m_iOSUtils->showActionSheet(title, message, buttons);
#else
    Q_UNUSED(title);
    Q_UNUSED(message);
    Q_UNUSED(buttons);
#endif // Q_OS_IOS
}

void NativeUtils::openGallery()
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->openGallery();
#endif // Q_OS_ANDROID
}

bool NativeUtils::isButtonEventsEnabled() const
{
#ifdef Q_OS_ANDROID
    return m_AndroidUtils->isButtonEventsEnabled();
#else
    return false;
#endif // Q_OS_ANDROID
}

void NativeUtils::setButtonEventsEnabled(bool enabled)
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->setButtonEventsEnabled(enabled);
#else
    Q_UNUSED(enabled);
#endif // Q_OS_ANDROID
}

bool NativeUtils::isEnabled() const
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->isEnabled();
#else
    return true;
#endif // Q_OS_ANDROID
}

void NativeUtils::setEnabled(bool enabled)
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->setEnabled(enabled);
#else
    Q_UNUSED(enabled);
#endif // Q_OS_ANDROID
}

}
