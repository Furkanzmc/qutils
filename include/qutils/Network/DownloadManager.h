#pragma once
// Qt
#include <QObject>
#include <QNetworkReply>

// Forward declerations
class QNetworkAccessManager;
class QSslError;
class QIODevice;

namespace zmc
{

namespace Network
{

/**
 * @brief The DownloadManager class
 * This is the class to to download any content from a URL. Right now, it is very primitive. It only downloads files to a directory.
 */
class DownloadManager: public QObject
{
    Q_OBJECT

public:
    explicit DownloadManager(QObject *parent = nullptr);
    ~DownloadManager();

    /**
     * @brief Downloadds a file to the given file path. File path must alos include the file name
     * @param url
     * @param filePath File path can also be just a directory.
     * @param downloadName Provide a custom name for the download.
     * @return void
     */
    Q_INVOKABLE void downloadFile(const QUrl &url, const QString &filePath = "", const QString &downloadName = "");

private:
    QNetworkAccessManager *m_NetworkManager;
    QList<QNetworkReply *> m_CurrentDownloads;

private:
    /**
     * @brief Saves the file to the disk.
     * @param filePath
     * @param data
     * @return bool
     */
    bool saveToDisk(const QString &filePath, QIODevice *data) const;

    /**
     * @brief Gets the file name from content disposition. If that fails, return a random file name.
     * @param reply
     * @return QString
     */
    QString getSaveFileName(const QNetworkReply *reply) const;

    void onProcessFinishedDownload(QNetworkReply *reply);

    /**
     * @brief Handles the download progress report from QNetworkReply. When a download starts, it will be binded with either the URL or the download name so
     * that we can report the progress of multiple downloads. Emits `progressChanged`.
     * @param downloadName
     * @param bytesReceived
     * @param bytesTotal
     */
    void onProgressChanged(const QString &downloadName, qint64 bytesReceived, qint64 bytesTotal);

private slots:
#ifndef QT_NO_SSL
    void sslErrors(const QList<QSslError> &errors);
#endif // QT_NO_SSL

signals:
    void allDownloadsFinished();

    /**
     * @brief Emitted when the download finishes.
     * @param url
     * @param filePath
     * @param downloadName
     */
    void downloadFinished(const QString &url, const QString &filePath, const QString &downloadName);

    /**
     * @brief Emitted when the progress of a download changes.
     * @param downloadName This will be the name of the download If it was given when adding the download. Otherwise it will be the download URL.
     * @param progress A value between 0 and 1. If the total size of the download cannot be determined, the value for this will be -1 as will the bytesTotal.
     * @param bytesReceived
     * @param bytesTotal
     */
    void progressChanged(const QString &downloadName, float progress, qint64 bytesReceived, qint64 bytesTotal);

    /**
     * @brief Emitted when an error occurrs while downlading.
     * @param downloadName
     * @param code
     */
    void errorOccurred(const QString &downloadName, QNetworkReply::NetworkError code);
};

}

}
