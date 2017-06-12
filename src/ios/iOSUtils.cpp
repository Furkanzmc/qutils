#include "qutils/ios/iOSUtils.h"
// Native
#include "qutils/ios/iOSNativeUtils.h"

namespace zmc
{

iOSUtils::iOSUtils(QObject *parent)
    : QObject(parent)
    , m_iOSNative(new iOSNativeUtils())
{
    m_iOSNative->onAlertDialogClicked = std::bind(&iOSUtils::alertDialogClicked, this, std::placeholders::_1);
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

}
