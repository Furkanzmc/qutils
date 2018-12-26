#pragma once
// Qt
#include <QObject>

// Forward declerations
class QNetworkAccessManager;

namespace zmc
{

namespace Network
{

// Forward declerations
class Download;

/*!
    \class DownloadManager
    \brief The DownloadManager class
   
    This is the class to to download any content from a URL. Right now, it is very primitive.
    It only downloads files to a directory.
*/
class DownloadManager: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int maxParallelDownload READ maxParallelDownload WRITE setMaxParallelDownload NOTIFY maxParallelDownloadChanged)
    Q_PROPERTY(int downloadCount READ downloadCount NOTIFY downloadCountChanged)

public:
    explicit DownloadManager(QObject *parent = nullptr);
    ~DownloadManager();

    /*!
        \brief Starts a downlaod for the given URL and returns the \c Download handle to it.
        \param url
        \return Download *

        Returned \c Download object belongs to this instance. When the download is finished,
        it is automatically deleted.
    */
    Q_INVOKABLE Download *downloadFile(const QUrl &url);
    Q_INVOKABLE QObject *download(const QUrl &url);

    /*!
        \property int DownloadManager::maxParallelDownload
    */
    int maxParallelDownload() const;
    void setMaxParallelDownload(int length);

    /*!
        \property int DownloadManager::downloadCount

        Returns the number of downloads that are in progress.
    */
    int downloadCount() const;

private:
    QNetworkAccessManager *m_NetworkManager;
    int m_MaxQueueLength;
    std::vector<Download *> m_CurrentDownloads;

    std::vector<Download *> m_Queue;

private:
    void onDownloadFinished(Download *download);
    void startDownload(Download *download);

signals:
    void allDownloadsFinished();
    void maxParallelDownloadChanged();
    void downloadCountChanged();
};

}

}
