#pragma once
#include <QObject>
#include <qqml.h>

namespace zmc
{

/**
 * @brief This namespace will contain some useful enums that are not directly exposed to QML by Qt.
 */
namespace CppTypes
{

Q_NAMESPACE

// The table is copied from here: http://doc.qt.io/qt-5/qnetworkreply.html#NetworkError-enum
enum NetworkError : unsigned int {
    NoError = 0,
    ConnectionRefusedError = 1,
    RemoteHostClosedError = 2,
    HostNotFoundError = 3,
    TimeoutError = 4,
    OperationCanceledError = 5,
    SslHandshakeFailedError = 6,
    TemporaryNetworkFailureError = 7,
    NetworkSessionFailedError = 8,
    BackgroundRequestNotAllowedError = 9,
    TooManyRedirectsError = 10,
    InsecureRedirectError = 11,
    ProxyConnectionRefusedError = 101,
    ProxyConnectionClosedError = 102,
    ProxyNotFoundError = 103,
    ProxyTimeoutError = 104,
    ProxyAuthenticationRequiredError = 105,
    ContentAccessDenied = 201,
    ContentOperationNotPermittedError = 202,
    ContentNotFoundError = 203,
    AuthenticationRequiredError = 204,
    ContentReSendError = 205,
    ContentConflictError = 206,
    ContentGoneError = 207,
    InternalServerError = 401,
    OperationNotImplementedError = 402,
    ServiceUnavailableError = 403,
    ProtocolUnknownError = 301,
    ProtocolInvalidOperationError = 302,
    UnknownNetworkError = 99,
    UnknownProxyError = 199,
    UnknownContentError = 299,
    ProtocolFailure = 399,
    UnknownServerError = 499
};
Q_ENUM_NS(NetworkError)

static void registerType()
{
    qmlRegisterUncreatableMetaObject(CppTypes::staticMetaObject, "qutils.CppTypes", 1, 0, "CppTypes", "Error: only enums");
}

}

}
