#include "qutils/Network/Download.h"
// Qt
#include <QDir>
#include <QFileInfo>
#include <QSaveFile>
#include <QNetworkReply>
#include <QRegularExpression>
// qutils
#include "qutils/Macros.h"
#include "qutils/FileUtils.h"

namespace zmc
{
namespace Network
{

Download::Download(QObject *parent)
    : QObject(parent)
    , m_Progress(0.0)
    , m_NetworkReply(nullptr)
    , m_TotalSize(0)
    , m_ReceivedSize(0)
    , m_IsInProgress(false)
    , m_QueueNumber(0)
{
}

QUrl Download::url() const
{
    return m_Url;
}

void Download::setUrl(const QUrl &_url)
{
    if (m_Url != _url) {
        m_Url = _url;
        emit urlChanged();
    }
}

QString Download::downloadFolder() const
{
    return m_DownloadFolder;
}

void Download::setDownloadFolder(const QString &path)
{
    QFileInfo fileInfo(path);
    const QString folderPath = fileInfo.absoluteFilePath();
    if (m_DownloadFolder != folderPath) {
        m_DownloadFolder = folderPath;
        emit downloadFolderChanged();

        if (fileInfo.isFile()) {
            setFileName(fileInfo.fileName());
        }
    }
}

QString Download::fileName() const
{
    return m_FileName;
}

void Download::setFileName(const QString &name)
{
    if (m_FileName != name) {
        m_FileName = name;
        emit fileNameChanged();
    }
}

qreal Download::progress() const
{
    return m_Progress;
}

void Download::setProgress(qreal prog)
{
    if (m_Progress != prog) {
        m_Progress = prog;
        emit progressChanged();
    }
}

qint64 Download::totalSize() const
{
    return m_TotalSize;
}

qint64 Download::receivedSize() const
{
    return m_ReceivedSize;
}

bool Download::inProgress() const
{
    return m_IsInProgress;
}

void Download::setInProgress(bool inprog)
{
    if (m_IsInProgress != inprog) {
        m_IsInProgress = inprog;
        emit inProgressChanged();
    }
}

QString Download::filePath() const
{
    return QString("%1/%2").arg(m_DownloadFolder).arg(m_FileName);
}

int Download::queueNumber() const
{
    return m_QueueNumber;
}

void Download::setQueueNumber(int number)
{
    if (m_QueueNumber != number) {
        m_QueueNumber = number;
        emit queueNumberChanged();
    }
}

QNetworkReply *Download::networkReply()
{
    return m_NetworkReply;
}

void Download::setNetworkReply(QNetworkReply *reply)
{
    assert(reply != nullptr);

    if (m_NetworkReply != reply) {
        m_NetworkReply = reply;
        setUrl(reply->url());

        connect(m_NetworkReply,
                QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
                [this](QNetworkReply::NetworkError error) {
                    setInProgress(false);
                    emit errorOccurred(error);
                });

        connect(m_NetworkReply, &QNetworkReply::downloadProgress, this, &Download::onProgressChanged);
        connect(m_NetworkReply, &QNetworkReply::redirected, this, &Download::redirected);
        connect(m_NetworkReply, &QNetworkReply::finished, this, &Download::onFinished);

        connect(m_NetworkReply, &QNetworkReply::redirected, [this]() {
            setUrl(m_NetworkReply->url());
        });

#ifndef QT_NO_SSL
        connect(m_NetworkReply, &QNetworkReply::sslErrors, this, &Download::sslErrors);
#endif // QT_NO_SSL
    }
}

void Download::onProgressChanged(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        setProgress(static_cast<qreal>(bytesReceived) / static_cast<qreal>(bytesTotal));
    }

    if (m_TotalSize == 0) {
        m_TotalSize = bytesTotal;
        setInProgress(true);
        setSaveFilePath();
        emit started();
        emit totalSizeChanged();
    }

    if (m_ReceivedSize != bytesReceived) {
        m_ReceivedSize = bytesReceived;
        emit receivedSizeChanged();
    }
}

#ifndef QT_NO_SSL
void Download::sslErrors(const QList<QSslError> &sslErrors)
{
    setInProgress(false);
    for (const QSslError &error : sslErrors) {
        LOG_ERROR("SSL error: " << error.errorString());
        emit sslErrorOcurred(error.error());
    }
}
#endif // QT_NO_SSL

bool Download::saveToDisk() const
{
    QSaveFile saveFile(filePath());
    if (!saveFile.open(QIODevice::WriteOnly)) {
        LOG_ERROR("Could not open" << saveFile.fileName() << " for writing: " << saveFile.errorString());
        return false;
    }

    saveFile.write(m_NetworkReply->readAll());
    saveFile.commit();
    return true;
}

void Download::setSaveFilePath()
{
    if (!m_DownloadFolder.isEmpty() && !m_FileName.isEmpty()) {
        return;
    }

    if (m_FileName.isEmpty()) {
        const QString contentDisposition = m_NetworkReply->rawHeader("Content-Disposition");
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
            setFileName(split.size() == 2 ? split.at(1) : split.at(0));
        }
        else {
            setFileName(m_NetworkReply->url().fileName());
        }
    }

    if (m_DownloadFolder.isEmpty()) {
        QString path = FileUtils::getTemporaryFile(m_FileName);
        setDownloadFolder(path.left(path.lastIndexOf(FILE_PATH_DIVIDER)));
    }
}

void Download::onFinished()
{
    if (saveToDisk()) {
        emit finished();
    }
}

} // namespace zmc
} // namespace Network
