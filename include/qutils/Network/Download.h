#pragma once
#include <QUrl>
#include <QObject>

class QIODevice;
class QNetworkReply;
#ifndef QT_NO_SSL
    class QSslError;
#endif // QT_NO_SSL

namespace zmc
{
namespace Network
{

/*!
    \class Download
    \brief Holds information about a download.
    \sa DownloadManager

    \c Download object is used by \c DownloadManager to queue and process download operations.

    TODO: Add speed reporting.
*/
class Download : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString downloadFolder READ downloadFolder WRITE setDownloadFolder NOTIFY downloadFolderChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)

    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(qint64 totalSize READ totalSize NOTIFY totalSizeChanged)
    Q_PROPERTY(qint64 receivedSize READ receivedSize NOTIFY receivedSizeChanged)

    Q_PROPERTY(bool inProgress READ inProgress NOTIFY inProgressChanged)
    Q_PROPERTY(QString filePath READ filePath CONSTANT)
    // TODO: Add a WRITE method here.
    Q_PROPERTY(int queueNumber READ queueNumber NOTIFY queueNumberChanged)

public:
    explicit Download(QObject *parent = nullptr);

    /*!
        \property url Download::url
    */
    QUrl url() const;
    void setUrl(const QUrl &_url);

    /*!
        \property QString Download::downloadFolder

        Download folder must be an absolute path.
    */
    QString downloadFolder() const;
    void setDownloadFolder(const QString &path);

    /*!
        \property QString Download::fileName
    */
    QString fileName() const;
    void setFileName(const QString &name);

    /*!
        \property qreal Download::progress
    */
    qreal progress() const;
    void setProgress(qreal prog);

    /*!
        \property qint64 Download::totalSize
    */
    qint64 totalSize() const;

    /*!
        \property qint64 Download::receivedSize
    */
    qint64 receivedSize() const;

    /*!
        \property bool Download::inProgress
    */
    bool inProgress() const;
    void setInProgress(bool inprog);

    /*!
        \property QString Download::filePath

        Returns the combination of download folder and file name.
    */
    QString filePath() const;

    /*!
        \property int Download::queueNumber

        Returns this download's place in the queue.
    */
    int queueNumber() const;
    void setQueueNumber(int number);

    QNetworkReply *networkReply();
    void setNetworkReply(QNetworkReply *reply);

private:
    QUrl m_Url;
    QString m_DownloadFolder;

    QString m_FileName;
    qreal m_Progress;
    QNetworkReply *m_NetworkReply;

    qint64 m_TotalSize, m_ReceivedSize;
    bool m_IsInProgress;
    int m_QueueNumber;

private:
    void onProgressChanged(qint64 bytesReceived, qint64 bytesTotal);
#ifndef QT_NO_SSL
    void sslErrors(const QList<QSslError> &errors);
#endif // QT_NO_SSL

    /*!
        \internal
        \brief Saves the file to the disk.
        \return bool
    */
    bool saveToDisk() const;

    /*!
        \internal
        \brief Gets the file name from content disposition. If that fails, return a random file name.
        \return void
    */
    void setSaveFilePath();

    void onFinished();

signals:
    void urlChanged();
    void downloadFolderChanged();
    void fileNameChanged();

    void progressChanged();
    void totalSizeChanged();
    void receivedSizeChanged();

    void inProgressChanged();
    /*!
        \brief Emitted when an error occurrs while downlading.
        \param code
    */
    void errorOccurred(int code);
    void sslErrorOcurred(int code);

    void finished();
    void started();
    void redirected(const QUrl &url);

    void queueNumberChanged();
};

}
}
