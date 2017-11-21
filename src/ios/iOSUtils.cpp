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
{
    m_iOSNative->onAlertDialogClicked = std::bind(&iOSUtils::alertDialogClicked, this, std::placeholders::_1);
    m_iOSNative->onActionSheetClicked = std::bind(&iOSUtils::actionSheetClicked, this, std::placeholders::_1);
    m_iOSNative->onKeyboardHeightChanged = std::bind(&iOSUtils::keyboardHeightChanged, this, std::placeholders::_1);

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

    m_Instances.push_back(this);
}

iOSUtils::~iOSUtils()
{
    m_Instances[m_InstanceID] = nullptr;
}

void iOSUtils::showAlertView(const QVariantMap &dialogProperties)
{
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
    m_iOSNative->requestLocationPermission();
}

int iOSUtils::getLocationAuthorizationStatus()
{
    return static_cast<int>(m_iOSNative->getLocationAuthorizationStatus());
}

void iOSUtils::emitOpenedWithURLSignal(const QString &url)
{
    if (m_Instances.size() == 0) {
        m_URLOpenedWith = url;
    }
    else {
        for (iOSUtils *utils : m_Instances) {
            if (utils == nullptr) {
                continue;
            }

            utils->openedWithURL(url);
        }
    }
}

void iOSUtils::emitOpenedWithoutURLSignal()
{
    if (m_Instances.size() > 0) {
        iOSUtils *utils = m_Instances.at(0);
        if (utils) {
            emit utils->openedWithoutURL();
        }
    }
}

}
