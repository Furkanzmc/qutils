#include "qutils/Miscellaneous.h"
// Qt
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QDateTime>
// Local
#include "qutils/Macros.h"

namespace zmc
{

ImageQualityWorkerThread::ImageQualityWorkerThread(const QString &imagePath, const int &quality, const QString &newPath, QObject *parent)
    : QThread(parent)
    , m_ImagePath(imagePath)
    , m_Quality(quality)
    , m_NewPath(newPath)
{
    connect(this, &ImageQualityWorkerThread::finished, this, &ImageQualityWorkerThread::deleteLater);
}

void ImageQualityWorkerThread::run()
{
    bool sucess = false;
    QFile file(m_NewPath);

    if (file.open(QIODevice::OpenModeFlag::WriteOnly) == false) {
        LOG_ERROR("Cannot open file for writing. Reason: " << file.errorString());
    }
    else {
        QImage img(m_ImagePath);
        img.save(&file, nullptr, m_Quality);
        file.close();
        sucess = true;
    }

    emit resultReady(sucess, m_NewPath);
}

// ----- Miscellaneous ----- //

Miscellaneous::Miscellaneous(QObject *parent)
    : QObject(parent)
{

}

void Miscellaneous::changeImageQuality(QString imagePath, QString newPath, const int &quality)
{
    if (imagePath.contains("file://")) {
        imagePath.remove("file://");
    }

    if (newPath.contains("file://")) {
        newPath.remove("file://");
    }

    ImageQualityWorkerThread *workerThread = new ImageQualityWorkerThread(imagePath, quality, newPath, this);
    connect(workerThread, &ImageQualityWorkerThread::resultReady, this, &Miscellaneous::imageQualityChanged);
    workerThread->start();
}

QVariantMap Miscellaneous::getFileInfo(QString filePath)
{
    if (filePath.contains("file://")) {
        filePath.remove("file://");
    }

    QVariantMap info;
    const QFileInfo fileInfo(filePath);
    if (fileInfo.exists() == false) {
        LOG_ERROR(filePath << " does not exist!");
    }

    info["absoluteFilePath"] = fileInfo.absoluteFilePath();
    info["baseName"] = fileInfo.baseName();
    info["completeBaseName"] = fileInfo.completeBaseName();

    info["completeSuffix"] = fileInfo.completeSuffix();
    info["created"] = fileInfo.created().toString(Qt::DateFormat::ISODate);
    info["fileName"] = fileInfo.fileName();

    info["size"] = fileInfo.size();
    info["absoluteDirPath"] = fileInfo.absoluteDir().absolutePath();

    return info;
}

}
