#pragma once
// Qt
#include <QObject>

// Forward declerations
class QNetworkAccessManager;
class QNetworkReply;
class QSslError;
class QIODevice;

namespace zmc
{

namespace Network
{

/**
 * @brief The DownloadManager class
 * This is the class to to download any content from a URL. Right now, it is very primitive. It only downloads files to a directory.
 * Downloads the files in parallel. So make sure the instance is still alive until after the downloads are finished.
 *
 * TODO:
 * [ ] Add progress reporting
 * [ ] Add error checking
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
     * @param filePath
     */
    void downloadFile(const QUrl &url, const QString &filePath = "");

    /**
     * @brief Uses the given URL to get the file name
     * @param url
     * @return
     */
    QString getSaveFileName(const QUrl &url) const;

private:
    QNetworkAccessManager *m_NetworkManager;
    QList<QNetworkReply *> m_CurrentDownloads;

private:
    bool saveToDisk(const QString &filePath, QIODevice *data) const;

private slots:
    void processFinishedDownload(QNetworkReply *reply);
#ifndef QT_NO_SSL
    void sslErrors(const QList<QSslError> &errors);
#endif // QT_NO_SSL

signals:
    void allDownloadsFinished();
    void downloadFinished(const QString &url, const QString &filePath);
};

}

}
