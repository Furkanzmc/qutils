#include "qutils/Network/DownloadManager.h"
// Qt
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
// Local
#include "qutils/Macros.h"
#include "qutils/Network/Download.h"

namespace zmc
{

namespace Network
{

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent)
    , m_NetworkManager(new QNetworkAccessManager(this))
    , m_MaxQueueLength(4)
{
}

DownloadManager::~DownloadManager()
{
}

int DownloadManager::maxParallelDownload() const
{
    return m_MaxQueueLength;
}

void DownloadManager::setMaxParallelDownload(int length)
{
    if (m_MaxQueueLength != length) {
        m_MaxQueueLength = length;
        emit maxParallelDownloadChanged();
    }
}

int DownloadManager::downloadCount() const
{
    return m_CurrentDownloads.size();
}

Download *DownloadManager::downloadFile(const QUrl &url)
{
    if (!url.isValid()) {
        LOG_ERROR("Given URL (" << url.toEncoded() << ") is not valid. Aborting download.");
        return nullptr;
    }

    Download *download = new Download(this);
    download->setUrl(url);

    if (static_cast<int>(m_CurrentDownloads.size()) < m_MaxQueueLength) {
        startDownload(download);
    }
    else {
        download->setQueueNumber(m_Queue.size() + 1);
        m_Queue.push_back(download);
    }

    return download;
}

QObject *DownloadManager::download(const QUrl &url)
{
    Download *download = downloadFile(url);
    return download ? qobject_cast<QObject *>(download) : nullptr;
}

void DownloadManager::onDownloadFinished(Download *download)
{
    auto it = std::find(m_CurrentDownloads.begin(), m_CurrentDownloads.end(), download);
    m_CurrentDownloads.erase(it);
    download->deleteLater();
    if (m_CurrentDownloads.empty()) {
        emit allDownloadsFinished();
    }
    else if (m_Queue.size() > 0) {
        startDownload(m_Queue.back());
        m_Queue.pop_back();
    }

    emit downloadCountChanged();
}

void DownloadManager::startDownload(Download *download)
{
    QNetworkRequest request(download->url());
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, QVariant(true));
    QNetworkReply *reply = m_NetworkManager->get(request);
    download->setNetworkReply(reply);
    connect(download, &Download::finished, std::bind(&DownloadManager::onDownloadFinished, this, download));

    m_CurrentDownloads.push_back(download);
    emit downloadCountChanged();
}

} // namespace Network
} // namespace zmc
