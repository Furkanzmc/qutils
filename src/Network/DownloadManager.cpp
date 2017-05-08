#include "qutils/Network/DownloadManager.h"
// qutils
#include "qutils/Macros.h"
// Qt
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#ifndef QT_NO_SSL
#include <QSslError>
#endif // QT_NO_SSL
#include <QUrl>

namespace zmc
{

namespace Network
{

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent)
    , m_NetworkManager(new QNetworkAccessManager(parent))
{
    connect(m_NetworkManager, &QNetworkAccessManager::finished, this, &DownloadManager::processFinishedDownload);
}

DownloadManager::~DownloadManager()
{
    m_NetworkManager->deleteLater();
}

void DownloadManager::downloadFile(const QUrl &url, const QString &filePath)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = m_NetworkManager->get(request);

#ifndef QT_NO_SSL
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
#endif // QT_NO_SSL

    reply->setProperty("file_name", filePath);
    m_CurrentDownloads.append(reply);
}

QString DownloadManager::getSaveFileName(const QUrl &url) const
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty()) {
        basename = "download";
    }

    if (QFile::exists(basename)) {
        // Already exists, don't overwrite
        unsigned int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i))) {
            ++i;
        }

        basename += QString::number(i);
    }

    return basename;
}

bool DownloadManager::saveToDisk(const QString &filePath, QIODevice *data) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR("Could not open" << filePath << " for writing: " << file.errorString());
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

#ifndef QT_NO_SSL

void DownloadManager::sslErrors(const QList<QSslError> &sslErrors)
{
    foreach (const QSslError &error, sslErrors) {
        LOG_ERROR("SSL error: " << error.errorString());
    }
}

#endif // QT_NO_SSL

void DownloadManager::processFinishedDownload(QNetworkReply *reply)
{
    const QUrl url = reply->url();
    if (reply->error()) {
        LOG("Download of " << url.toEncoded() << " failed: " << reply->errorString());
    }
    else {
        QString filePath = reply->property("file_name").toString();
        if (filePath.length() == 0) {
            filePath = getSaveFileName(url);
        }
        else {
            // If the suffix was omitteed in the custom file name, get the suffix from the URL and append to the file name
            QFileInfo fileInfo(filePath);
            if (fileInfo.suffix().length() == 0) {
                fileInfo.setFile(getSaveFileName(url));
                filePath += "." + fileInfo.suffix();
            }
        }

        if (saveToDisk(filePath, reply)) {
            LOG("Download of " << url.toEncoded() << " succeeded. Saved to " << filePath);
            emit downloadFinished(url.toString(), filePath);
        }
    }

    m_CurrentDownloads.removeAll(reply);
    reply->deleteLater();

    if (m_CurrentDownloads.size() == 0) {
        emit allDownloadsFinished();
    }
}

}

}
