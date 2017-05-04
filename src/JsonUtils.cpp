#include "qutils/JsonUtils.h"
// Qt
#include <QJsonDocument>
#include <QJsonObject>
#include "QJsonArray"
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

QString JsonUtils::toJsonString(const QList<QVariant> &data)
{
    QJsonDocument doc;
    QJsonArray array;
    for (int i = 0; i < data.size(); i++) {
        array.append(QJsonValue::fromVariant(data.at(i)));
    }

    doc.setArray(array);

    return doc.toJson(QJsonDocument::JsonFormat::Compact);;
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

QList<QVariant> JsonUtils::toVariantList(const QString &data)
{
    QList<QVariant> list;
    const QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    if (doc.isNull() == false && doc.isArray()) {
        const QJsonArray array = doc.array();
        const int arraySize = array.size();

        for (int i = 0; i < arraySize; i++) {
            list.append(array.at(i).toObject().toVariantMap());
        }
    }
    else {
        LOG_ERROR("Given data is nat valid to convert to QList<QVariantMap>.");
    }

    return list;
}

}
