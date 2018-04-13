#pragma once
#include <QObject>
#include <QThread>
#include <QVariantMap>

namespace zmc
{

#if defined(Q_OS_IOS)
class FileUtilsPrivate;
#endif // Q_OS_IOS

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
    void resultReady(bool success, const QString &savedPath);

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
    ~FileUtils();

    /**
     * @brief Changes the image quality without altering the image size. The quality factor must be in the range 0 to 100 or -1. Specify 0 to obtain small
     * compressed files, 100 for large uncompressed files, and -1 (the default) to use the default settings. After changing the quality, the image is
     * overwritten If newPath variable is not set. But for any reason the newPath cannot opened for writing, the operation will be terminated.
     * The quality factor must be in the range 0 to 100 or -1. Specify 0 to obtain small compressed files, 100 for large uncompressed files, and -1
     * (the default) to use the default settings.
     * @param imagePath
     * @param quality
     * @return void
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
     * @return QVariantMap
     */
    Q_INVOKABLE QVariantMap getFileInfo(QString filePath);

    /**
     * @brief Deletes the given file from the file systen and returns true if succeeds.
     * @param filePath
     * @return bool
     */
    Q_INVOKABLE bool remove(QString filePath);

    /**
     * @brief Returns true if the file exists.
     * @param filePath
     * @return bool
     */
    Q_INVOKABLE bool exists(QString filePath);

    /**
     * @brief Copies the given file to given location.
     * @param filePath
     * @return bool
     */
    Q_INVOKABLE bool copy(QString filePath, QString newFilePath);

    /**
     * @brief Returns a checksum of the file using MD5 algorithm.
     * @param filePath
     * @return QString
     */
    Q_INVOKABLE QString getFileChecksum(QString filePath);

    /**
     * @brief Returns true if the URL is non-empty and valid; otherwise returns false. Calls the QUrl::isValid.
     * @param url
     * @return bool
     */
    Q_INVOKABLE bool isValidURL(const QString &url) const;

    /**
     * @brief Returns true if this URL is pointing to a local file path. A URL is a local file path if the scheme is "file". Calls the QUrl::isLocalFile.
     * @param url
     * @return bool
     */
    Q_INVOKABLE bool isLocalFile(const QString &url) const;

    /**
     * @brief See FileUtilsPrivate::openDocumentPicker()
     * @return bool
     */
    Q_INVOKABLE bool openDocumentPicker(const QStringList &documentTypes, bool selectMultiple = false);

    /**
     * @brief Uses QTemporaryFile to generate a temporary file in the Temporary files directory of the current platform. The file template is used to generate
     * a unique name. The file is not automatically removed from the temp directory.
     * @param directory
     * @return
     */
    Q_INVOKABLE static QString getTemporaryFile(const QString &fileTemplate);

    /**
     * @brief Reads the file at the given path. If the file does not exist or cannot be reached it returns an empty string. Otherwise the contents of the file
     * are returned in UTF-8 encoding.
     * @param filePath
     * @return QString
     */
    Q_INVOKABLE static QString readFile(QString filePath);

signals:
    void imageQualityChanged(bool success, const QString &savedPath);
    void documentPickerCanceled();
    void documentPicked(const QStringList &paths);

private:
#if defined(Q_OS_IOS)
    FileUtilsPrivate *m_FileUtilsPrivate;
#endif // Q_OS_IOS

private:
    void showImage(int num);
    void finished();
};

}
