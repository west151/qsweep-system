#include "qsweeprequest.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUuid>

QSweepRequest::QSweepRequest(QObject *parent) : QObject(parent)
{
    m_id = QUuid::createUuid().toString().mid(1, 8);
}

QSweepRequest::QSweepRequest(const QByteArray &json, bool binary)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    QJsonObject obj = doc.object();
    m_id = obj["id"].toString();
}

QString QSweepRequest::idRequest() const
{
    return m_id;
}

QByteArray QSweepRequest::exportToJson() const
{
    QJsonObject jsonObject;
    jsonObject["id"] = m_id;

    QJsonDocument doc(jsonObject);

    return doc.toJson(QJsonDocument::Compact);
}

QByteArray QSweepRequest::exportToJsonBinary() const
{
    QJsonObject jsonObject;
    jsonObject["id"] = m_id;

    QJsonDocument doc(jsonObject);

    return doc.toBinaryData();
}
