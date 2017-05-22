#include "qutils/Network/NetworkManager.h"
// Qt
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpPart>
#include <QFile>
#include <QMimeDatabase>
// qutils
#include "qutils/Macros.h"

namespace zmc
{

namespace Network
{

unsigned int NetworkManager::m_RequestCount = 0;

NetworkManager::NetworkManager()
    : QObject(nullptr)
{
    connect(&m_Network, &QNetworkAccessManager::finished, this, &NetworkManager::onRequestFinished);
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::sendGet(const QString &url, RequestCallback callback)
{
    const int availableIndex = getAvailableIndex();
    const unsigned int threadIndex = availableIndex == -1 ? m_Callbacks.size() : availableIndex;
    const QUrl qurl = QUrl(url);
    QNetworkRequest request(qurl);
    setHeaders(request);
    QNetworkReply *reply = m_Network.get(request);

    reply->setObjectName(QString::number(threadIndex));
    insertCallback(threadIndex, std::move(callback));
}

void NetworkManager::sendDelete(const QString &url, RequestCallback callback)
{
    const int availableIndex = getAvailableIndex();
    const unsigned int threadIndex = availableIndex == -1 ? m_Callbacks.size() : availableIndex;
    const QUrl qurl = QUrl(url);
    QNetworkRequest request(qurl);
    setHeaders(request);
    QNetworkReply *reply = m_Network.deleteResource(request);

    reply->setObjectName(QString::number(threadIndex));
    insertCallback(threadIndex, std::move(callback));
}

void NetworkManager::sendPost(const QString &url, const QString &data, RequestCallback callback)
{
    const int availableIndex = getAvailableIndex();
    const unsigned int threadIndex = availableIndex == -1 ? m_Callbacks.size() : availableIndex;
    const QUrl qurl = QUrl(url);
    QNetworkRequest request(qurl);
    setHeaders(request);

    const QByteArray postData = data.toUtf8();
    request.setHeader(QNetworkRequest::KnownHeaders::ContentLengthHeader, postData.size());
    QNetworkReply *reply = m_Network.post(request, postData);

    reply->setObjectName(QString::number(threadIndex));
    insertCallback(threadIndex, std::move(callback));
}

void NetworkManager::sendPut(const QString &url, const QString &data, RequestCallback callback)
{
    const int availableIndex = getAvailableIndex();
    const unsigned int threadIndex = availableIndex == -1 ? m_Callbacks.size() : availableIndex;
    const QUrl qurl = QUrl(url);
    QNetworkRequest request(qurl);
    setHeaders(request);
    const QByteArray putData = data.toUtf8();
    request.setHeader(QNetworkRequest::KnownHeaders::ContentLengthHeader, putData.size());

    QNetworkReply *reply = m_Network.put(request, putData);

    reply->setObjectName(QString::number(threadIndex));
    insertCallback(threadIndex, std::move(callback));
}

void NetworkManager::uploadFiles(const QString &url, const QMap<QString, QString> &files, const QMap<QString, QString> &textParams, RequestCallback callback)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QMimeDatabase db;

    for (auto it = files.constBegin(); it != files.constEnd(); it++) {
        QString filePath = it.value();
        if (filePath.contains("file:///")) {
            filePath.remove("file:///");
        }

        QFile *file = new QFile(filePath);
        if (file->exists() == false) {
            continue;
        }
        const QString contentType = db.mimeTypeForFile(filePath).name();
        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
        const QString contentDisposition = QString("form-data; name=\"%1\"; filename=\"%2\"").arg(it.key()).arg(file->fileName());
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, contentDisposition);

        file->open(QIODevice::ReadOnly);
        filePart.setBodyDevice(file);
        filePart.setHeader(QNetworkRequest::ContentLengthHeader, file->size());
        // We cannot delete the file now, so delete it with the multiPart
        file->setParent(multiPart);

        multiPart->append(filePart);
    }

    for (auto it = textParams.constBegin(); it != textParams.constEnd(); it++) {
        QHttpPart textPart;
        const QString contentDisposition = QString("form-data; name=\"%1\";").arg(it.key());
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, contentDisposition);
        textPart.setBody(it.value().toUtf8());
        multiPart->append(textPart);
    }

    const QUrl qurl = QUrl(url);
    QNetworkRequest request(qurl);
    setHeaders(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + QString(multiPart->boundary()));

    QNetworkReply *reply = m_Network.post(request, multiPart);
    // Delete the multiPart with the reply
    multiPart->setParent(reply);

    const int availableIndex = getAvailableIndex();
    const unsigned int threadIndex = availableIndex == -1 ? m_Callbacks.size() : availableIndex;
    reply->setObjectName(QString::number(threadIndex));
    insertCallback(threadIndex, std::move(callback));

    connect(reply, &QNetworkReply::uploadProgress, this, &NetworkManager::onUploadProgressChanged);
}

bool NetworkManager::isConnectedToInternet()
{
    // FIXME: Use platform specific code to achieve this.
    return true;
}

int NetworkManager::getNextrequestID()
{
    m_RequestCount++;
    return m_RequestCount;
}

void NetworkManager::setHeader(const QString &headerName, const QString &headerValue)
{
    m_Headers[headerName.toUtf8()] = headerValue.toUtf8();
}

void NetworkManager::removeHeader(const QString &headerName)
{
    m_Headers.remove(headerName.toUtf8());
}

void NetworkManager::onReceivedResponse(const Response &response, int threadIndex)
{
    if (threadIndex >= m_Callbacks.size()) {
        return;
    }

    RequestCallback &callback = m_Callbacks[threadIndex];
    if (callback) {
        callback(response);
    }

    m_Callbacks[threadIndex] = nullptr;
}

void NetworkManager::onUploadProgressChanged(qint64 bytesSent, qint64 bytesTotal)
{
    float percent = 0.f;
    if (bytesTotal > 0) {
        percent = static_cast<float>(bytesSent) / static_cast<float>(bytesTotal);
    }

    emit uploadProgressChanged(bytesSent, bytesTotal, percent);
}

int NetworkManager::getAvailableIndex()
{
    auto foundIt = std::find_if(m_Callbacks.begin(), m_Callbacks.end(), [](std::function<void(const Response &)> cb) {
        return cb == nullptr;
    });

    if (foundIt == m_Callbacks.end()) {
        return -1;
    }

    return std::distance(m_Callbacks.begin(), foundIt);
}

void NetworkManager::insertCallback(const int &threadIndex, RequestCallback &&callback)
{
    if (threadIndex < m_Callbacks.size()) {
        m_Callbacks[threadIndex] = callback;
    }
    else {
        m_Callbacks.push_back(callback);
    }
}

void NetworkManager::setHeaders(QNetworkRequest &request)
{
    for (auto it = m_Headers.constBegin(); it != m_Headers.constEnd(); it++) {
        request.setRawHeader(it.key(), it.value());
    }
}

void NetworkManager::onRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        LOG_ERROR("Network error occured for URL (" << reply->request().url().toString() << ")."
                  "\nError string: " << reply->errorString() <<
                  "\nError code: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
    }

    const Response response(reply->readAll(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());

    bool intConversionOk = false;
    int callbackIndex = reply->objectName().toInt(&intConversionOk);
    if (intConversionOk == false) {
        callbackIndex = -1;
    }

    onReceivedResponse(response, callbackIndex);
}

}

}
