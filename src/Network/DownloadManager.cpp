#include "qutils/Network/DownloadManager.h"
// Qt
#include <QUrl>
#include <QFile>
#include <QList>
#include <QSaveFile>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QNetworkAccessManager>
#include <QRegularExpressionMatch>
#ifndef QT_NO_SSL
    #include <QSslError>
#endif // QT_NO_SSL
// Local
#include "qutils/Macros.h"
#include "qutils/FileUtils.h"

namespace zmc
{

namespace Network
{

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent)
    , m_NetworkManager(new QNetworkAccessManager(this))
{
    connect(m_NetworkManager, &QNetworkAccessManager::finished, this, &DownloadManager::onProcessFinishedDownload);
}

DownloadManager::~DownloadManager()
{
    m_NetworkManager->deleteLater();
}

void DownloadManager::downloadFile(const QUrl &url, const QString &filePath, const QString &downloadName)
{
    if (!url.isValid()) {
        LOG_ERROR("Given URL (" << url.toEncoded() << ") is not valid. Aborting download.");
        return;
    }

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, QVariant(true));
    QNetworkReply *reply = m_NetworkManager->get(request);

#ifndef QT_NO_SSL
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));
#endif // QT_NO_SSL

    reply->setProperty("file_path", filePath);
    reply->setProperty("download_name", downloadName);

    const QString downloadID = downloadName.length() > 0 ? downloadName : url.toEncoded();
    connect(
        reply,
        &QNetworkReply::downloadProgress,
        std::bind(&DownloadManager::onProgressChanged, this, downloadID, std::placeholders::_1, std::placeholders::_2)
    );

    connect(
        reply,
        QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
        std::bind(&DownloadManager::errorOccurred, this, downloadID, std::placeholders::_1)
    );

    m_CurrentDownloads.append(reply);
}

QString DownloadManager::getSaveFileName(const QNetworkReply *reply) const
{
    QString filePath = reply->property("file_path").toString();
    const QFileInfo fileInfo(filePath);

    if (filePath.isEmpty() || fileInfo.isDir()) {
        const QString contentDisposition = reply->rawHeader("Content-Disposition");
        QRegularExpression re("filename[^;=\n]*=(UTF-8(['\"]*))?(.*)");
        QRegularExpressionMatch match = re.match(contentDisposition);
        if (match.hasMatch()) {
            QString matched = match.captured(0);
            if (matched.contains("filename*=UTF-8''")) {
                matched.remove("UTF-8''");
                matched.remove("filename*");
            }

            if (matched.contains("filename=")) {
                matched.remove("filename=");
                matched.remove("\"");
            }

            const QStringList split = matched.split("=");
            const QString fileName = split.size() == 2 ? split.at(1) : split.at(0);
            filePath = FileUtils::getTemporaryFile(fileName, fileInfo.isDir() ? fileInfo.absoluteFilePath() : "");
        }
        else {
            const QUrl url = reply->url();
            filePath = FileUtils::getTemporaryFile(url.fileName(), fileInfo.isDir() ? fileInfo.absoluteFilePath() : "");
        }
    }

    return filePath;
}

bool DownloadManager::saveToDisk(const QString &filePath, QIODevice *data) const
{
    QSaveFile saveFile(filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        LOG_ERROR("Could not open" << filePath << " for writing: " << saveFile.errorString());
        return false;
    }

    saveFile.write(data->readAll());
    saveFile.commit();
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

void DownloadManager::onProcessFinishedDownload(QNetworkReply *reply)
{
    const QUrl url = reply->url();
    if (reply->error()) {
        LOG("Download of " << url.toEncoded() << " failed: " << reply->errorString());
    }
    else {
        const QString filePath = getSaveFileName(reply);

        if (saveToDisk(filePath, reply)) {
            LOG("Download of " << url.toEncoded() << " succeeded. Saved to " << filePath);
            const QString downloadName = reply->property("download_name").toString();
            emit downloadFinished(url.toEncoded(), filePath, downloadName);
        }
    }

    m_CurrentDownloads.removeAll(reply);
    reply->deleteLater();
    if (m_CurrentDownloads.isEmpty()) {
        emit allDownloadsFinished();
    }
}

void DownloadManager::onProgressChanged(const QString &downloadName, qint64 bytesReceived, qint64 bytesTotal)
{
    // If the bytesTotal is -1, then the total size of the download cannot be determined.
    float progress = static_cast<float>(bytesReceived) / static_cast<float>(bytesTotal);
    if (progress < 0) {
        progress = -1;
    }

    emit progressChanged(downloadName, progress, bytesReceived, bytesTotal);
}

}

}
