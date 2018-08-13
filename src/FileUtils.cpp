#include "qutils/FileUtils.h"
// Qt
#include <QCryptographicHash>
#include <QTemporaryFile>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDateTime>
#include <QImage>
#include <QFile>
#include <QDir>
#include <QUrl>
// Local
#if defined(Q_OS_IOS)
    #include "qutils/ios/FileUtils_Private.h"
    #include "qutils/ios/iOSUtils.h"
#elif defined(Q_OS_ANDROID)
    #include "qutils/android/AndroidUtils.h"
#endif // Q_OS_ANDRID
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

    if (!file.open(QIODevice::OpenModeFlag::WriteOnly)) {
        LOG_ERROR("Cannot open file for writing. Reason: " << file.errorString());
    }
    else {
        QImage img(m_ImagePath);
        success = img.save(&file, nullptr, m_Quality);
        file.close();
    }

    emit resultReady(success, m_NewPath);
}

// ----- FileInfo ----- //

FileInfo::FileInfo(QObject *parent)
    : QObject(parent)
{

}

bool FileInfo::exists() const
{
    return m_Exists;
}

void FileInfo::setExists(bool ex)
{
    m_Exists = ex;
}

QString FileInfo::absoluteFilePath() const
{
    return m_AbsoluteFilePath;
}

void FileInfo::setAbsoluteFilePath(const QString &path)
{
    m_AbsoluteFilePath = path;
}

QString FileInfo::baseName() const
{
    return m_BaseName;
}

void FileInfo::setBaseName(const QString &name)
{
    m_BaseName = name;
}

QString FileInfo::completeBaseName() const
{
    return m_CompleteBaseName;
}

void FileInfo::setCompleteBaseName(const QString &name)
{
    m_CompleteBaseName = name;
}

QString FileInfo::completeSuffix() const
{
    return m_CompleteSuffix;
}

void FileInfo::setCompleteSuffix(const QString &suffix)
{
    m_CompleteSuffix = suffix;
}

QDateTime FileInfo::created() const
{
    return m_Created;
}

void FileInfo::setCreated(const QDateTime &dt)
{
    m_Created = dt;
}

QString FileInfo::fileName() const
{
    return m_FileName;
}

void FileInfo::setFileName(const QString &name)
{
    m_FileName = name;
}

qint64 FileInfo::size() const
{
    return m_Size;
}

void FileInfo::setSize(const qint64 &sz)
{
    m_Size = sz;
}

QString FileInfo::absoluteDirPath() const
{
    return m_AbsoluteDirPath;
}

void FileInfo::setAbsoluteDirPath(const QString &path)
{
    m_AbsoluteDirPath = path;
}

void FileInfo::reset()
{
    m_AbsoluteFilePath = "";
    m_BaseName = "";
    m_CompleteBaseName = "";
    m_CompleteSuffix = "";
    m_FileName = "";
    m_AbsoluteDirPath = "";

    m_Exists = false;
    m_Size = 0;
    m_Created = QDateTime();
}


// ----- FileUtils ----- //

FileUtils::FileUtils(QObject *parent)
    : QObject(parent)
#if defined(Q_OS_IOS)
    , m_FileUtilsPrivate(new FileUtilsPrivate())
    , m_iOSUtils(new iOSUtils(this))
#elif defined(Q_OS_ANDROID)
    , m_AndroidUtils(new AndroidUtils(this))
#endif // Q_OS_IOS
    , m_FileInfo(nullptr)
{
#if defined(Q_OS_IOS)
    m_FileUtilsPrivate->onDocumentPickerCanceled = std::bind(&FileUtils::documentPickerCanceled, this);
    m_FileUtilsPrivate->onDocumentPicked = std::bind(&FileUtils::documentPicked, this, std::placeholders::_1);

    connect(m_iOSUtils, &iOSUtils::imageSelected, this, &FileUtils::photoSelected);
    connect(m_iOSUtils, &iOSUtils::imageSelectionCancelled, this, &FileUtils::photoSelectionCanceled);
#elif defined(Q_OS_ANDROID)
    connect(m_AndroidUtils, &AndroidUtils::photoSelected, this, &FileUtils::photoSelected);
    connect(m_AndroidUtils, &AndroidUtils::photoSelectionCanceled, this, &FileUtils::photoSelectionCanceled);

    connect(m_AndroidUtils, &AndroidUtils::fileSelected, this, &FileUtils::documentPicked);
    connect(m_AndroidUtils, &AndroidUtils::fileSelectionCancelled, this, &FileUtils::documentPickerCanceled);
#endif // Q_OS_ANDROID
}

FileUtils::~FileUtils()
{
#if defined(Q_OS_IOS)
    delete m_FileUtilsPrivate;
#endif // Q_OS_IOS
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

QObject *FileUtils::getFileInfo(QString filePath)
{
    if (filePath.contains(FILE_PATH_PREFIX)) {
        filePath.remove(FILE_PATH_PREFIX);
    }

    if (m_FileInfo == nullptr) {
        m_FileInfo = new FileInfo(this);
    }

    m_FileInfo->reset();
    const QFileInfo fileInfo(filePath);
    m_FileInfo->setExists(fileInfo.exists());

    if (fileInfo.exists()) {
        m_FileInfo->setAbsoluteFilePath(fileInfo.absoluteFilePath());
        m_FileInfo->setBaseName(fileInfo.baseName());
        m_FileInfo->setCompleteBaseName(fileInfo.completeBaseName());

        m_FileInfo->setCompleteSuffix(fileInfo.completeSuffix());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        m_FileInfo->setCreated(fileInfo.birthTime());
#else
        m_FileInfo->setCreated(fileInfo.created());
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))

        m_FileInfo->setFileName(fileInfo.fileName());
        m_FileInfo->setSize(fileInfo.size());
        m_FileInfo->setAbsoluteDirPath(fileInfo.absoluteDir().absolutePath());
    }

    return static_cast<QObject *>(m_FileInfo);
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

bool FileUtils::openDocumentPicker(const QStringList &documentTypes, bool selectMultiple)
{
    bool opened = false;
#if defined(Q_OS_IOS)
    opened = m_FileUtilsPrivate->openDocumentPicker(documentTypes, selectMultiple);
#elif defined(Q_OS_ANDROID)
    Q_UNUSED(selectMultiple);
    opened = true;
    m_AndroidUtils->openDocumentPicker(documentTypes.join("|"));
#else
    Q_UNUSED(selectMultiple);
    Q_UNUSED(documentTypes);
#endif // Q_OS_IOS

    return opened;
}

void FileUtils::openGallery()
{
#if defined(Q_OS_ANDROID)
    m_AndroidUtils->openGallery();
#elif defined(Q_OS_IOS)
    m_iOSUtils->openGallery();
#endif // Q_OS_ANDROID
}

QString FileUtils::getTemporaryFile(const QString &fileTemplate, const QString &directory)
{
    QString tmpPath;
    if (directory.length() > 0 && QDir(directory).exists()) {
        tmpPath = directory;
    }
    else {
        const QStringList paths = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
        tmpPath = paths.at(0);
    }

    QTemporaryFile tmpFile;
    tmpFile.setFileTemplate(tmpPath + "/" + fileTemplate);

    tmpFile.open();
    QString filePath = tmpFile.fileName();

    if (filePath.contains(fileTemplate)) {
        filePath = tmpPath + "/" + fileTemplate;
        tmpFile.setAutoRemove(true);
    }
    else {
        tmpFile.setAutoRemove(false);
    }

    tmpFile.close();
    return filePath;
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
    if (!file.exists()) {
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
