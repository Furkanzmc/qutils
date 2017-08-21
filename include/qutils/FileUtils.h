#pragma once
#include <QObject>
#include <QThread>
#include <QVariantMap>

namespace zmc
{

class ImageQualityWorkerThread : public QThread
{
    Q_OBJECT

public:
    ImageQualityWorkerThread(const QString &imagePath, const int &quality, const QString &newPath = "", QObject *parent = nullptr);

    void run() override;
    QString ImagePath() const;
    void setImagePath(const QString &path);

    QString NewPath() const;
    void setNewPath(const QString &NewPath);

signals:
    void resultReady(bool sucess, const QString &savedPath);

private:
    QString m_ImagePath;
    int m_Quality;
    QString m_NewPath;
};

/**
 * @brief The FileUtils class
 * This class holds functionality that is commonly used for files.
 */
class FileUtils : public QObject
{
    Q_OBJECT

public:
    FileUtils(QObject *parent = nullptr);

    /**
     * @brief Changes the image quality without altering the image size. The quality factor must be in the range 0 to 100 or -1. Specify 0 to obtain small
     * compressed files, 100 for large uncompressed files, and -1 (the default) to use the default settings. After changing the quality, the image is
     * overwritten If newPath variable is not set. But for any reason the newPath cannot opened for writing, the operation will be terminated.
     * @param imagePath
     * @param quality
     * @return bool
     */
    Q_INVOKABLE void changeImageQuality(QString imagePath, QString newPath, const int &quality);

    /**
     * @brief Returns some selected information about the file. If the file does not exist, an empty dictionary is returned. Here is the information contained
     * in the dictionary:
     * - absoluteFilePath
     * - baseName
     * - completeBaseName
     * - completeSuffix
     * - created: The time format is in Qt::DateFormat::ISODate
     * - fileName
     * - size
     * - absoluteDirPath: The result of fileInfo.absoluteDir().absolutePath()
     * - exists: If this is false, all the other fields will be missing.
     * You can access them with the same names using the JSON dictionary. (e.g result.fileName)
     * @param filePath
     * @return
     */
    Q_INVOKABLE QVariantMap getFileInfo(QString filePath);

signals:
    void imageQualityChanged(bool sucess, const QString &savedPath);

private:
    void showImage(int num);
    void finished();
};

}
