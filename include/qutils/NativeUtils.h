#pragma once
#include <QObject>
#include <QColor>
// qutils
#include "qutils/Macros.h"
#ifdef Q_OS_ANDROID
// qutils
#include "qutils/android/AndroidUtils.h"
#endif // Q_OS_ANDRID

namespace zmc
{

/**
 * @brief The NativeUtils class is used as a main source of entry to both Android and iOS features. When a feature is not supported on the current development
 * platform, the feature will be disabled but it will also not cause a compilation error.
 */
class NativeUtils : public QObject
{
    Q_OBJECT

public:
    explicit NativeUtils(QObject *parent = 0);

    Q_INVOKABLE void setStatusBarColor(QColor color);
    Q_INVOKABLE void setStatusBarVisible(bool visible);
    Q_INVOKABLE void setImmersiveMode(bool visible);

signals:
    void backButtonPressed();
    void menuButtonPressed();

private:
#ifdef Q_OS_ANDROID
    static AndroidUtils *m_AndroidUtils;
#endif // Q_OS_ANDROID

private:
    void emitBackButtonPressed();
    void emitMenuButtonPressed();
};

}
