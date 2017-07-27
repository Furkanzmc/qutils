#pragma once
// std
#include <functional>
// Qt
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace zmc
{

namespace Network
{

struct Response {
    Response(QString _data, unsigned int _httpCode)
        : data(_data)
        , httpCode(_httpCode)
    {

    }

    QString data;
    unsigned int httpCode;
};

using RequestCallback = std::function<void(const Response &)>;

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    NetworkManager();
    ~NetworkManager();

    void sendGet(const QString &url, RequestCallback callback);
    void sendDelete(const QString &url, RequestCallback callback);
    void sendPost(const QString &url, const QString &data, RequestCallback callback);

    void sendPut(const QString &url, const QString &data, RequestCallback callback);

    /**
     * @brief Uploads the given files with HTTP multipart.
     * @param url
     * @param files QMap<UPLOAD_KEY, UPLOAD_FILE>
     * @param textParams QMap<UPLOAD_KEY, UPLOAD_VALUE>
     * @param callback
     */
    void uploadFiles(const QString &url, const QMap<QString, QString> &files, const QMap<QString, QString> &textParams, RequestCallback callback);
    bool isConnectedToInternet();

    /**
     * @brief Increases m_RequestCount and returns the resulting ID
     * @return
     */
    int getNextrequestID();

    /**
     * @brief When a header is set, it is used for all of the requests. If a header with the same headerName exists, it is overwritten.
     * @param headerName
     * @param headerValue
     */
    void setHeader(const QString &headerName, const QString &headerValue);

    void removeHeader(const QString &headerName);

private:
    static unsigned int m_RequestCount;

    QNetworkAccessManager m_Network;
    QList<RequestCallback> m_Callbacks;
    QMap<QByteArray, QByteArray> m_Headers;

signals:
    void uploadProgressChanged(qint64 bytesSent, qint64 bytesTotal, float percent);

private:
    void onRequestFinished(QNetworkReply *reply);
    void onReceivedResponse(const Response &response, int threadIndex);
    void onUploadProgressChanged(qint64 bytesSent, qint64 bytesTotal);

    /**
     * @brief Returns the first nullptr thread index. If none found, returns -1
     * @return
     */
    int getAvailableIndex();

    void insertCallback(const int &threadIndex, RequestCallback &&callback);

    /**
     * @brief If a token exists, sets the Authorization header of the HTTPRequest
     * @param request
     */
    void setHeaders(QNetworkRequest &request);
};

}

}
