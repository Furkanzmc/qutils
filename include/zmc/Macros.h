#pragma once
#include <qsystemdetection.h>

#if defined(Q_OS_WIN) || defined(Q_OS_MAC) || (defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID))
#define Q_OS_DESKTOP
#endif // Desktop Platform

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#define Q_OS_MOBILE
#endif // Desktop Platform
