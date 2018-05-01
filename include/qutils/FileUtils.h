#pragma once
// Qt
#include <QVariantMap>
#include <QObject>
#include <QThread>

namespace zmc
{

#if defined(Q_OS_IOS)
class FileUtilsPrivate;
class iOSUtils;
#elif defined(Q_OS_ANDROID)
class AndroidUtils;
#endif // Q_OS_ANDRID

class ImageQualityWorkerThread : public QThread
{
    Q_OBJECT

public:
    ImageQualityWorkerThread(const QString &imagePath, const int &quality, const QString &newPath = "", QObject *parent = nullptr);

    void run() override;
    QString imagePath() const;
    void setImagePath(const QString &path);

    QString newPath() const;
    void setNewPath(const QString &newPath);

signals:
    void resultReady(bool success, const QString &savedPath);

private:
    QString m_ImagePath;
    int m_Quality;
    QString m_NewPath;
};

/**
 * @brief The FileUtils class
 * This class holds functionality that is commonly used for files and selection of files.
 *
 * You can open the gallery or open a document browser on iOS and Android.
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
     * @brief Opens the UIDocumentPickerViewController on iOS and the default file browser on Android.
     * See FileUtilsPrivate::openDocumentPicker() for details on iOS.
     * documentTypes can be left blank on iOS and it will default to `public.data` but on Android, it is mandatory.
     * @param documentTypes This is the UTI on iOS and MIME type on Android.
     * @param selectMultiple
     * @return bool
     */
    Q_INVOKABLE bool openDocumentPicker(const QStringList &documentTypes, bool selectMultiple = false);

    /**
     * @brief Opens the gallery on iOS and Android. On Android, the MIME type defaults to image/\*. If you want to select files, you can call the
     * openDocumentPicker() method.
     */
    Q_INVOKABLE void openGallery();

    /**
     * @brief Uses QTemporaryFile to generate a temporary file in the Temporary files directory of the current platform. The file template is used to generate
     * a unique name. The file is not automatically removed from the temp directory.
     * @param directory
     * @return QString
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
    /**
     * @brief Emitted when the image's quality is changed.
     * @param success
     * @param savedPath
     */
    void imageQualityChanged(bool success, const QString &savedPath);

    /**
     * @brief Emitted on iOS when the DocumentPickerView is canceled.
     */
    void documentPickerCanceled();

    /**
     * @brief Emitted on iOS when the user selects document(s) using the DocumentPickerView. If selectMultiple multiple was set to true, then you'll get the
     * paths to all of the files.
     * @param paths
     */
    void documentPicked(const QStringList &paths);

    /**
     * @brief Emitted when the user selectes a photo from the gallery.
     * @param photoPath
     */
    void photoSelected(const QString &photoPath);

    /**
     * @brief Emitted when the user cancels the photo selection.
     */
    void photoSelectionCanceled();

private:
#if defined(Q_OS_IOS)
    FileUtilsPrivate *m_FileUtilsPrivate;
    iOSUtils *m_iOSUtils;
#elif defined(Q_OS_ANDROID)
    AndroidUtils *m_AndroidUtils;
#endif // Q_OS_ANDROID

private:
    void showImage(int num);
    void finished();
};

}
