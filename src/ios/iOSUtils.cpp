#include "qutils/ios/iOSUtils.h"
// Qt
#include <QTimer>
// Local
#include "qutils/ios/iOSNativeUtils.h"
#include "qutils/Macros.h"

namespace zmc
{

QList<iOSUtils *> iOSUtils::m_Instances = QList<iOSUtils *>();
QString iOSUtils::m_URLOpenedWith = "";

iOSUtils::iOSUtils(QObject *parent)
    : QObject(parent)
    , m_InstanceID(m_Instances.size())
    , m_iOSNative(new iOSNativeUtils())
    , m_IsMainController(false)
{
    m_iOSNative->onAlertDialogClicked = std::bind(&iOSUtils::alertDialogClicked, this, std::placeholders::_1);
    m_iOSNative->onActionSheetClicked = std::bind(&iOSUtils::actionSheetClicked, this, std::placeholders::_1);
    m_iOSNative->onKeyboardHeightChanged = std::bind(&iOSUtils::keyboardHeightChanged, this, std::placeholders::_1);

    m_iOSNative->onImagePickerControllerCancelled = std::bind(&iOSUtils::imagePickerCancelledCallback, this);
    m_iOSNative->onImagePickerControllerFinishedPicking = std::bind(&iOSUtils::imagePickerFinishedPickingCallback, this, std::placeholders::_1);
    m_iOSNative->onCameraCancelled = std::bind(&iOSUtils::cameraCancelledCallback, this);

    m_iOSNative->onPhotosAccessGranted = std::bind(&iOSUtils::photosAccessGranted, this);
    m_iOSNative->onPhotosAccessDenied = std::bind(&iOSUtils::photosAccessDenied, this);

    if (isMainController()) {
        if (m_URLOpenedWith.length() > 0) {
            /*
             * This is to execute the `emitOpenedWithURLSignals` function with the next cycle.
             * This is used to make sure that NativeUtils and AndroidUtils can catch the emitted signal.
             */
            QTimer::singleShot(1, std::bind(iOSUtils::emitOpenedWithURLSignal, m_URLOpenedWith));
            m_URLOpenedWith = "";
        }
        else {
            /*
             * This is to execute the `emitOpenedWithURLSignals` function with the next cycle.
             * This is used to make sure that NativeUtils and AndroidUtils can catch the emitted signal.
             */
            QTimer::singleShot(1, std::bind(iOSUtils::emitOpenedWithoutURLSignal));
        }
    }

    m_Instances.push_back(this);
}

iOSUtils::~iOSUtils()
{
    m_Instances[m_InstanceID] = nullptr;
    delete m_iOSNative;
}

void iOSUtils::showAlertView(const QVariantMap &dialogProperties)
{
    if (this->signalsBlocked()) {
        return;
    }

    const QVariantList varButtons = dialogProperties.value("buttons").toList();
    QStringList buttons;
    for (const QVariant &button : varButtons) {
        buttons.append(button.toString());
    }

    m_iOSNative->showAlertView(dialogProperties.value("title", "").toString(), dialogProperties.value("message", "").toString(), buttons);
}

void iOSUtils::shareText(const QString &text)
{
    m_iOSNative->shareText(text);
}

void iOSUtils::showActionSheet(const QString &title, const QString &message, const QVariantList &buttons)
{
    if (this->signalsBlocked()) {
        return;
    }

    m_iOSNative->showActionSheet(title, message, buttons);
}

void iOSUtils::dismissKeyboard()
{
    m_iOSNative->dismissKeyboard();;
}

void iOSUtils::setApplicationIconBadgeNumber(const int &number)
{
    m_iOSNative->setApplicationIconBadgeNumber(number);
}

bool iOSUtils::isiPad() const
{
    return m_iOSNative->isiPad();
}

void iOSUtils::openSafari(const QString &url)
{
    m_iOSNative->openSafari(url);
}

void iOSUtils::requestLocationPermission()
{
    if (this->signalsBlocked()) {
        return;
    }

    m_iOSNative->requestLocationPermission();
}

void iOSUtils::requestPhotosPermisson()
{
    if (this->signalsBlocked()) {
        return;
    }

    m_iOSNative->requestPhotosPermisson();
}

int iOSUtils::getLocationAuthorizationStatus()
{
    return static_cast<int>(m_iOSNative->getLocationAuthorizationStatus());
}

int iOSUtils::getPhotosAuthorizationStatus()
{
    return static_cast<int>(m_iOSNative->getPhotosAuthorizationStatus());
}

bool iOSUtils::isLocationAuthorizationGranted() const
{
    return m_iOSNative->getLocationAuthorizationStatus() == iOSNativeUtils::LAAuthorizedAlways
           || m_iOSNative->getLocationAuthorizationStatus() == iOSNativeUtils::LAAuthorizedWhenInUse;
}

bool iOSUtils::isPhotosPermissionGranted() const
{
    return m_iOSNative->getPhotosAuthorizationStatus() == iOSNativeUtils::PAAuthorized;
}

void iOSUtils::openGallery()
{
    if (this->signalsBlocked()) {
        return;
    }

    m_iOSNative->openGallery();
}

void iOSUtils::showCamera()
{
    if (this->signalsBlocked()) {
        return;
    }

    m_iOSNative->showCamera();
}

void iOSUtils::emitOpenedWithURLSignal(QString url)
{
    if (m_Instances.size() == 0) {
        m_URLOpenedWith = url;
    }
    else {
        const int currentCount = m_Instances.count();
        for (int index = 0; index < currentCount; index++) {
            iOSUtils *utils = m_Instances.at(index);
            if (utils && utils->isMainController()) {
                emit utils->openedWithURL(url);
                break;
            }
        }
    }
}

void iOSUtils::emitOpenedWithoutURLSignal()
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        iOSUtils *utils = m_Instances.at(index);
        if (utils && utils->isMainController()) {
            emit utils->openedWithoutURL();
            break;
        }
    }
}

bool iOSUtils::isMainController() const
{
    return m_IsMainController;
}

void iOSUtils::setMainController(bool isMain)
{
    const int currentCount = m_Instances.count();
    for (int index = 0; index < currentCount; index++) {
        iOSUtils *utils = m_Instances.at(index);
        if (utils && utils->isMainController()) {
            utils->setMainController(false);
            break;
        }
    }

    if (m_IsMainController != isMain) {
        emit mainControllerChanged();
    }

    m_IsMainController = isMain;
}

bool iOSUtils::isEnabled() const
{
    return this->signalsBlocked();
}

void iOSUtils::setEnabled(bool enabled)
{
    if (!this->signalsBlocked() != enabled) {
        emit enabledChanged();
    }

    this->blockSignals(!enabled);
}

bool iOSUtils::isStatusBarVisible() const
{
    return m_iOSNative->isStatusBarVisible();
}

void iOSUtils::setStatusBarVisible(bool visible)
{
    m_iOSNative->setStatusBarVisible(visible);
}

void iOSUtils::imagePickerCancelledCallback()
{
    emit imageSelectionCancelled();
}

void iOSUtils::imagePickerFinishedPickingCallback(const QVariantMap &data)
{
    if (data["isSourceCamera"].toBool() == true) {
        emit cameraCaptured(data["tempUrl"].toString());
    }
    else {
        emit imageSelected(data["tempUrl"].toString());
    }
}

void iOSUtils::cameraCancelledCallback()
{
    emit cameraCaptureCancelled();
}

}
