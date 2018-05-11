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
    Response(QString _data, int _httpCode, QMap<QString, QByteArray> _headers, QNetworkReply::NetworkError error)
        : data(_data)
        , httpCode(_httpCode)
        , networkError(error)
        , headers(_headers)
    {

    }

    QString data;
    int httpCode;
    QNetworkReply::NetworkError networkError;
    QMap<QString, QByteArray> headers;
};

using RequestCallback = std::function<void(const Response &)>;
using UploadProgressCallback = std::function<void(qint64/*bytesSent*/, qint64/*bytesTotal*/)>;

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    /**
     * @brief Sends a get request. When the request is finished, the callback is called. If the queryParams parameter is provided, the query parameters are
     * appended to the end of the URL.
     * @param url
     * @param queryParams
     * @param callback
     */
    void sendGet(const QString &url, RequestCallback callback, const QVariantMap &queryParams = QVariantMap());

    /**
     * @brief Sends a head request. When the request is finished, the callback is called. If the queryParams parameter is provided, the query parameters are
     * appended to the end of the URL.
     * @param url
     * @param queryParams
     * @param callback
     */
    void sendHead(const QString &url, RequestCallback callback, const QVariantMap &queryParams = QVariantMap());

    /**
     * @brief Sends a delete request. When the request is finished, the callback is called.
     * @param url
     * @param callback
     */
    void sendDelete(const QString &url, RequestCallback callback);

    /**
     * @brief Sends a post request. When the request is finished, the callback is called.
     * @param url
     * @param data
     * @param callback
     */
    void sendPost(const QString &url, const QString &data, RequestCallback callback);

    /**
     * @brief Sends a put request. When the request is finished, the callback is called.
     * @param url
     * @param data
     * @param callback
     */
    void sendPut(const QString &url, const QString &data, RequestCallback callback);

    /**
     * @brief Uploads the given files with HTTP multipart.
     * @param url
     * @param files QMap<UPLOAD_KEY, UPLOAD_FILE>
     * @param textParams QMap<UPLOAD_KEY, UPLOAD_VALUE>
     * @param callback
     */
    void sendMultipartRequest(
        const QString &url,
        const QMap<QString, QString> &files,
        const QMap<QString, QString> &textParams,
        RequestCallback callback,
        UploadProgressCallback uploadProgressCallback = nullptr,
        bool usePutRequest = false
    );
    void sendMultipartPost(
        const QString &url,
        const QMap<QString, QString> &files,
        const QMap<QString, QString> &textParams,
        RequestCallback callback,
        UploadProgressCallback uploadProgressCallback = nullptr
    );
    void sendMultipartPut(
        const QString &url,
        const QMap<QString, QString> &files,
        const QMap<QString, QString> &textParams,
        RequestCallback callback,
        UploadProgressCallback uploadProgressCallback = nullptr
    );

    /**
     * @brief Uses the sendMultipartRequest to upload the given files.
     * @param url
     * @param files
     * @param textParams
     * @param callback
     */
    void uploadFiles(
        const QString &url,
        const QMap<QString, QString> &files,
        const QMap<QString, QString> &textParams,
        RequestCallback callback,
        UploadProgressCallback uploadProgressCallback = nullptr
    );

    /**
     * @brief Returns true if connected to internet.
     * @todo Fix this function. Currently it always returns true.
     * @return bool
     */
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

    /**
     * @brief Removes the header with the given name.
     * @param headerName
     */
    void removeHeader(const QString &headerName);

private:
    static int m_RequestCount;

    QNetworkAccessManager m_Network;
    QList<RequestCallback> m_Callbacks;
    QMap<QByteArray, QByteArray> m_Headers;

signals:
    void uploadProgressChanged(qint64 bytesSent, qint64 bytesTotal, float percent);

private:
    void onRequestFinished(QNetworkReply *reply);
    QMap<QString, QByteArray> getResponseHeaders(const QNetworkReply *reply);
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
