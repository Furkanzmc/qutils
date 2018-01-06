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
    , m_iOSUtils(new iOSUtils(this))
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
    connect(m_AndroidUtils, &AndroidUtils::mainControllerChanged, this, &NativeUtils::mainControllerChanged);
#endif // Q_OS_ANDROID

#ifdef Q_OS_IOS
    connect(m_iOSUtils, &iOSUtils::alertDialogClicked, this, &NativeUtils::alertDialogClicked);
    connect(m_iOSUtils, &iOSUtils::actionSheetClicked, this, &NativeUtils::actionSheetClicked);
    connect(m_iOSUtils, &iOSUtils::keyboardHeightChanged, this, &NativeUtils::keyboardHeightChanged);
    connect(m_iOSUtils, &iOSUtils::openedWithURL, this, &NativeUtils::openedWithURL);

    connect(m_iOSUtils, &iOSUtils::openedWithoutURL, this, &NativeUtils::openedWithoutURL);
    connect(m_iOSUtils, &iOSUtils::imageSelected, this, &NativeUtils::fileSelected);
    connect(m_iOSUtils, &iOSUtils::imageSelectionCancelled, this, &NativeUtils::fileSelectionCancelled);

    connect(m_iOSUtils, &iOSUtils::mainControllerChanged, this, &NativeUtils::mainControllerChanged);
    connect(m_iOSUtils, &iOSUtils::cameraCaptured, this, &NativeUtils::cameraCaptured);
    connect(m_iOSUtils, &iOSUtils::cameraCaptureCancelled, this, &NativeUtils::cameraCaptureCancelled);
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
    m_iOSUtils->setStatusBarVisible(visible);
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
#if defined(Q_OS_ANDROID)
    m_AndroidUtils->showCamera(photoName);
#elif defined(Q_OS_IOS)
    m_iOSUtils->showCamera();
    Q_UNUSED(photoName);
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

void NativeUtils::openGallery(const QString &fileType)
{
#if defined(Q_OS_ANDROID)
    m_AndroidUtils->openGallery(fileType);
#elif defined(Q_OS_IOS)
    m_iOSUtils->openGallery();
    Q_UNUSED(fileType);
#else
    Q_UNUSED(fileType);
#endif // Q_OS_ANDROID
}

void NativeUtils::dismissKeyboard()
{
#ifdef Q_OS_ANDROID
    m_AndroidUtils->dismissKeyboard();
#endif // Q_OS_ANDROID

#ifdef Q_OS_IOS
    m_iOSUtils->dismissKeyboard();
#endif // Q_OS_IOS
}

void NativeUtils::setApplicationIconBadgeNumber(const unsigned int &number)
{
#ifdef Q_OS_IOS
    m_iOSUtils->setApplicationIconBadgeNumber(number);
#else
    Q_UNUSED(number);
#endif // Q_OS_IOS
}

bool NativeUtils::isiPad() const
{
#ifdef Q_OS_IOS
    return m_iOSUtils->isiPad();
#else
    return false;
#endif // Q_OS_IOS
}

void NativeUtils::openSafari(const QString &url)
{
#ifdef Q_OS_IOS
    m_iOSUtils->openSafari(url);
#else
    Q_UNUSED(url);
#endif // Q_OS_IOS
}

QString NativeUtils::getDeviceModel()
{
    QString model = "";
#ifdef Q_OS_ANDROID
    model = m_AndroidUtils->getDeviceModel();
#endif // Q_OS_ANDROID

#ifdef Q_OS_WINDOWS
    model = "windows";
#endif // Q_OS_WINDOWS

#ifdef Q_OS_MAC
    model = "macOS";
#endif // Q_OS_WINDOWS

    return model;
}

void NativeUtils::requestLocationPermission()
{
#ifdef Q_OS_IOS
    m_iOSUtils->requestLocationPermission();
#endif // Q_OS_IOS
}

int NativeUtils::getLocationAuthorizationStatus()
{
    int status = 0;
#ifdef Q_OS_IOS
    status = m_iOSUtils->getLocationAuthorizationStatus();
#endif // Q_OS_IOS

    return status;
}

bool NativeUtils::isStatusBarVisible() const
{
    bool visible = false;
#ifdef Q_OS_IOS
    visible = m_iOSUtils->isStatusBarVisible();
#endif // Q_OS_IOS

#ifdef Q_OS_ANDROID
    visible = m_AndroidUtils->isStatusBarVisible();
#endif // Q_OS_ANDROID

    return visible;
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
    return m_AndroidUtils->isEnabled();
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

bool NativeUtils::isMainController() const
{
#if defined(Q_OS_IOS)
    return m_iOSUtils->isMainController();
#elif defined(Q_OS_ANDROID)
    return m_AndroidUtils->isMainController();
#endif // Q_OS_ANDROID

    return false;
}

void NativeUtils::setMainController(bool isMain)
{
#ifdef Q_OS_IOS
    m_iOSUtils->setMainController(isMain);
#elif defined(Q_OS_ANDROID)
    m_AndroidUtils->setMainController(isMain);
#else
    Q_UNUSED(isMain);
#endif // Q_OS_ANDROID
}

}
