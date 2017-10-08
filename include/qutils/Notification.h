#pragma once
#include "qutils/Macros.h"

#ifdef Q_OS_ANDROID
#include "android/Notification_Android.h"
#endif // Q_OS_ANDROID

#ifdef Q_OS_IOS
#include "ios/Notification_iOS.h"
#endif // Q_OS_ANDROID
