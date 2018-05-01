#include "qutils/QutilsLog.h"
// Local
#include "qutils/Macros.h"

namespace zmc
{

QutilsLog::QutilsLog(QObject *parent)
    : QObject(parent)
{

}

void QutilsLog::info(const QString &msg)
{
    LOG_JNI(msg.toStdString().c_str());
}

void QutilsLog::warning(const QString &msg)
{
    LOG_WARNING_JNI(msg.toStdString().c_str());
}

void QutilsLog::error(const QString &msg)
{
    LOG_ERROR_JNI(msg.toStdString().c_str());
}

}
