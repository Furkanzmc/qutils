#include "qutils/FileUtils.h"
// Qt
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QDateTime>
#include <QCryptographicHash>
#include <QUrl>
#include <QTemporaryFile>
#include <QStandardPaths>
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
    bool success = false;
    QFile file(m_NewPath);

    if (file.open(QIODevice::OpenModeFlag::WriteOnly) == false) {
        LOG_ERROR("Cannot open file for writing. Reason: " << file.errorString());
    }
    else {
        QImage img(m_ImagePath);
        img.save(&file, nullptr, m_Quality);
        file.close();
        success = true;
    }

    emit resultReady(success, m_NewPath);
}

// ----- FileUtils ----- //

FileUtils::FileUtils(QObject *parent)
    : QObject(parent)
{

}

void FileUtils::changeImageQuality(QString imagePath, QString newPath, const int &quality)
{
    if (imagePath.contains(FILE_PATH_PREFIX)) {
        imagePath.remove(FILE_PATH_PREFIX);
    }

    if (newPath.contains(FILE_PATH_PREFIX)) {
        newPath.remove(FILE_PATH_PREFIX);
    }

    ImageQualityWorkerThread *workerThread = new ImageQualityWorkerThread(imagePath, quality, newPath, this);
    connect(workerThread, &ImageQualityWorkerThread::resultReady, this, &FileUtils::imageQualityChanged);
    workerThread->start();
}

QVariantMap FileUtils::getFileInfo(QString filePath)
{
    if (filePath.contains(FILE_PATH_PREFIX)) {
        filePath.remove(FILE_PATH_PREFIX);
    }

    QVariantMap info;
    const QFileInfo fileInfo(filePath);
    if (fileInfo.exists() == false) {
        info["exists"] = false;
    }
    else {
        info["absoluteFilePath"] = fileInfo.absoluteFilePath();
        info["baseName"] = fileInfo.baseName();
        info["completeBaseName"] = fileInfo.completeBaseName();

        info["completeSuffix"] = fileInfo.completeSuffix();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        info["created"] = fileInfo.birthTime().toString(Qt::DateFormat::ISODate);
#else
        info["created"] = fileInfo.created().toString(Qt::DateFormat::ISODate);
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        info["fileName"] = fileInfo.fileName();

        info["size"] = fileInfo.size();
        info["absoluteDirPath"] = fileInfo.absoluteDir().absolutePath();
    }

    return info;
}

bool FileUtils::remove(QString filePath)
{
    if (filePath.contains(FILE_PATH_PREFIX)) {
        filePath.remove(FILE_PATH_PREFIX);
    }

    bool succeeded = false;
    QFile file(filePath);
    if (file.exists()) {
        succeeded = file.remove();
    }

    return succeeded;
}

bool FileUtils::exists(QString filePath)
{
    if (filePath.contains(FILE_PATH_PREFIX)) {
        filePath.remove(FILE_PATH_PREFIX);
    }

    QFile file(filePath);
    return file.exists();
}

bool FileUtils::copy(QString filePath, QString newFilePath)
{
    if (filePath.contains(FILE_PATH_PREFIX)) {
        filePath.remove(FILE_PATH_PREFIX);
    }

    if (newFilePath.contains(FILE_PATH_PREFIX)) {
        newFilePath.remove(FILE_PATH_PREFIX);
    }

    return QFile::copy(filePath, newFilePath);
}

QString FileUtils::getFileChecksum(QString filePath)
{
    if (filePath.contains(FILE_PATH_PREFIX)) {
        filePath.remove(FILE_PATH_PREFIX);
    }

    QString hashData = "";
    QFile file(filePath);

    if (file.exists()) {
        if (file.open(QFile::ReadOnly)) {
            QCryptographicHash hash(QCryptographicHash::Algorithm::Md5);
            if (hash.addData(&file)) {
                hashData = QString(hash.result().toHex());
            }
        }
        else {
            LOG_ERROR(filePath << " cannot be opened for reading. " << file.errorString());
        }
    }
    else {
        LOG_ERROR(filePath << " does not exist!");
    }

    return hashData;
}

bool FileUtils::isValidURL(const QString &url) const
{
    QUrl urlObject(url);
    return urlObject.isValid();
}

bool FileUtils::isLocalFile(const QString &url) const
{
    QUrl urlObject(url);
    return urlObject.isLocalFile();
}

QString FileUtils::getTemporaryFile(const QString &fileTemplate)
{
    QTemporaryFile tmpFile;
    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
    QString tmpPath = paths.at(0);

    tmpFile.setFileTemplate(tmpPath + "/" + fileTemplate);
    tmpFile.setAutoRemove(false);

    tmpFile.open();
    const QString fileName = tmpFile.fileName();
    tmpFile.close();

    return fileName;
}

QString FileUtils::readFile(QString filePath)
{
    if (filePath.length() == 0) {
        LOG_ERROR("Cannot read an empty file path.");
        return "";
    }

    if (filePath.contains(FILE_PATH_PREFIX)) {
        filePath.remove(FILE_PATH_PREFIX);
    }

    QFile file(filePath);
    if (file.exists() == false) {
        LOG_ERROR("File does not exist at " << filePath);
        return "";
    }

    QString content = "";
    if (file.open(QIODevice::ReadOnly)) {
        content = QString(file.readAll());
    }

    return content;
}

}
