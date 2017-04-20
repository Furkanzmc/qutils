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

void NativeUtils::emitBackButtonPressed()
{
    emit backButtonPressed();
}

void NativeUtils::emitMenuButtonPressed()
{
    emit menuButtonPressed();
}

}
