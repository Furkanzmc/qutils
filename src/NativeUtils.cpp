#include "qutils/NativeUtils.h"

#include <QDebug>

namespace zmc
{

#ifdef Q_OS_ANDROID
AndroidUtils *NativeUtils::m_AndroidUtils = new AndroidUtils();
#endif // Q_OS_ANDROID

NativeUtils::NativeUtils(QObject *parent)
    : QObject(parent)
{
#ifdef Q_OS_ANDROID
    connect(m_AndroidUtils, &AndroidUtils::backButtonPressed, this, &NativeUtils::emitBackButtonPressed);
    connect(m_AndroidUtils, &AndroidUtils::menuButtonPressed, this, &NativeUtils::emitMenuButtonPressed);
    connect(m_AndroidUtils, &AndroidUtils::alertDialogClicked, this, &NativeUtils::emitAlertDialogClicked);

    connect(m_AndroidUtils, &AndroidUtils::alertDialogCancelled, this, &NativeUtils::emitAlertDialogCancelled);
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

void NativeUtils::emitBackButtonPressed()
{
    emit backButtonPressed();
}

void NativeUtils::emitMenuButtonPressed()
{
    emit menuButtonPressed();
}

void NativeUtils::emitAlertDialogClicked(int buttonType)
{
    emit alertDialogClicked(buttonType);
}

void NativeUtils::emitAlertDialogCancelled()
{
    emit alertDialogCancelled();
}

}
