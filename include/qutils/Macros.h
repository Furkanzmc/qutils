#pragma once
#include <QDebug>
#include <qsystemdetection.h>

#if (!defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)) && (defined(Q_OS_WIN) || defined(Q_OS_MAC))
#define Q_OS_DESKTOP
#endif // Desktop Platform

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#define Q_OS_MOBILE
#endif // Desktop Platform

#ifdef Q_OS_MOBILE
#define LOG(msg) qDebug() << "[INFO] " << msg
#define LOG_WARNING(msg) qDebug() << "[WARNING] " << msg
#define LOG_ERROR(msg) qDebug() << "[ERROR] " << msg
#else
#define LOG(msg) qDebug() << "[INFO]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg
#define LOG_WARNING(msg) qDebug() << "[WARNING]" << __FUNCTION__ << ":" << __LINE__ << "):" << msg
#define LOG_ERROR(msg) qDebug() << "[ERROR]" << __FUNCTION__ << "(" << __LINE__ << "):" << msg
#endif // Q_OS_MOBILE

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
