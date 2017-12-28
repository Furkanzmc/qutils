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
    bool hasValidData = false;
    QList<QVariant> list;

    if (data.length() > 0) {
        const QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());

        if (doc.isNull() == false && doc.isArray()) {
            hasValidData = true;
            const QJsonArray array = doc.array();
            const int arraySize = array.size();

            for (int i = 0; i < arraySize; i++) {
                const QJsonValue value = array.at(i);
                if (value.isArray()) {
                    list.append(value.toArray().toVariantList());
                }
                else if (value.isBool()) {
                    list.append(value.toBool());
                }
                else if (value.isDouble()) {
                    list.append(value.toDouble());
                }
                else if (value.isNull()) {
                    list.append(QVariant());
                }
                else if (value.isObject()) {
                    list.append(value.toObject().toVariantMap());
                }
                else if (value.isString()) {
                    list.append(value.toString());
                }
                else if (value.isUndefined()) {
                    list.append(QVariant());
                }
            }
        }
    }

    if (hasValidData == false) {
        LOG_ERROR("Given data is nat valid to convert to QList<QVariant>: " << data);
    }

    return list;
}

}
