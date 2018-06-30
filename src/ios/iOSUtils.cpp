#include "qutils/ios/iOSUtils.h"
// Qt
#include <QTimer>
// Local
#include "qutils/Macros.h"
#include "qutils/ios/iOSNativeUtils.h"

namespace zmc
{

QMap<int, iOSUtils *> iOSUtils::m_Instances = QMap<int, iOSUtils *>();
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

    m_iOSNative->onImagePickerControllerCancelled = std::bind(&iOSUtils::imageSelectionCancelled, this);
    m_iOSNative->onImagePickerControllerFinishedPicking = std::bind(&iOSUtils::imagePickerFinishedPickingCallback, this, std::placeholders::_1);
    m_iOSNative->onCameraCancelled = std::bind(&iOSUtils::cameraCaptureCancelled, this);

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

    m_Instances.insert(m_InstanceID, this);
}

iOSUtils::~iOSUtils()
{
    m_Instances.remove(m_InstanceID);
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

void iOSUtils::showActionSheet(const QString &title, const QString &message, const QVariantList &buttons, QRect rect)
{
    if (this->signalsBlocked()) {
        return;
    }

    m_iOSNative->showActionSheet(title, message, buttons, rect);
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

void iOSUtils::openGallery()
{
    if (this->signalsBlocked()) {
        return;
    }

    m_iOSNative->openGallery();
}

QString iOSUtils::getDeviceName() const
{
    return m_iOSNative->getDeviceName();
}

QSize iOSUtils::getStatusBarSize() const
{
    return m_iOSNative->getStatusBarSize();
}

QColor iOSUtils::getStatusBarColor() const
{
    return m_iOSNative->getStatusBarColor();
}

void iOSUtils::openAppSettings() const
{
    m_iOSNative->openAppSettings();;
}

void iOSUtils::setStatusBarColor(const QColor &color)
{
    m_iOSNative->setStatusBarColor(color);
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
        auto begin = m_Instances.begin();
        auto end = m_Instances.end();
        for (auto it = begin; it != end; it++) {
            iOSUtils *utils = it.value();
            if (utils && utils->isMainController()) {
                QMetaObject::invokeMethod(utils, std::bind(&iOSUtils::openedWithURL, utils, url), Qt::QueuedConnection);
                break;
            }
        }
    }
}

void iOSUtils::emitOpenedWithoutURLSignal()
{
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        iOSUtils *utils = it.value();
        if (utils && utils->isMainController()) {
            QMetaObject::invokeMethod(utils, std::bind(&iOSUtils::openedWithoutURL, utils), Qt::QueuedConnection);
            break;
        }
    }
}

bool iOSUtils::isMainController() const
{
    return m_IsMainController;
}

void iOSUtils::setMainController(bool isMain, bool disableOthers)
{
    if (disableOthers) {
        auto begin = m_Instances.begin();
        auto end = m_Instances.end();
        for (auto it = begin; it != end; it++) {
            iOSUtils *utils = it.value();
            if (utils && utils->isMainController()) {
                utils->setMainController(false, false);
                break;
            }
        }
    }

    if (m_IsMainController != isMain) {
        emit mainControllerChanged();
        m_IsMainController = isMain;
    }
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

void iOSUtils::imagePickerFinishedPickingCallback(const QVariantMap &data)
{
    if (data["isSourceCamera"].toBool() == true) {
        emit cameraCaptured(data["tempUrl"].toString());
    }
    else {
        emit imageSelected(data["tempUrl"].toString());
    }
}

}
