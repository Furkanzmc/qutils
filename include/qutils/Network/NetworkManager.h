#pragma once
// std
#include <functional>
// Qt
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Utils
{

struct ApiResponse {
    ApiResponse(QString _data, unsigned int _httpCode)
        : data(_data)
        , httpCode(_httpCode)
    {

    }

    QString data;
    unsigned int httpCode;
};

using RequestCallback = std::function<void(const ApiResponse &)>;

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

    bool isConnectedToInternet();

    void setApiVersion(const QString &version);
    const QString &getAPIVersion() const;

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

private:
    QNetworkAccessManager m_Network;
    QList<RequestCallback> m_Callbacks;
    QMap<QByteArray, QByteArray> m_Headers;

    static unsigned int m_RequestCount;

private slots:
    void onRequestFinished(QNetworkReply *reply);

private:
    void onReceivedResponse(const ApiResponse &response, int threadIndex);

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
