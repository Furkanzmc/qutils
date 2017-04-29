#include "qutils/JsonUtils.h"
// Qt
#include <QJsonDocument>
#include <QJsonObject>
// qutils
#include "qutils/Macros.h"

namespace zmc
{

JsonUtils::JsonUtils()
{

}

QString JsonUtils::toJsonString(const QVariantMap &data)
{
    const QJsonObject &obj = QJsonObject::fromVariantMap(data);
    QJsonDocument doc;
    doc.setObject(obj);
    return doc.toJson(QJsonDocument::JsonFormat::Compact);
}

QString JsonUtils::toJsonString(const QVariant &data)
{
    const QJsonDocument doc = data.toJsonDocument();
    return doc.toJson(QJsonDocument::JsonFormat::Compact);
}

QVariantMap JsonUtils::toVariantMap(const QString &data)
{
    QVariantMap map;
    const QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    if (doc.isNull() == false) {
        map = doc.object().toVariantMap();
    }

    return map;
}

}
