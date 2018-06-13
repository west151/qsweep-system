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
    m_typeRequest = static_cast<TypeRequest>(obj["type_request"].toInt(0));
}

QString QSweepRequest::idRequest() const
{
    return m_id;
}

void QSweepRequest::setTypeRequest(const TypeRequest &value)
{
    m_typeRequest = value;
}

TypeRequest QSweepRequest::typeRequest() const
{
    return m_typeRequest;
}

QByteArray QSweepRequest::exportToJson() const
{
    QJsonObject jsonObject;
    jsonObject["id"] = m_id;
    jsonObject["type_request"] = static_cast<qint32>(m_typeRequest);

    QJsonDocument doc(jsonObject);

    return doc.toJson(QJsonDocument::Compact);
}

QByteArray QSweepRequest::exportToJsonBinary() const
{
    QJsonObject jsonObject;
    jsonObject["id"] = m_id;
    jsonObject["type_request"] = static_cast<qint32>(m_typeRequest);

    QJsonDocument doc(jsonObject);

    return doc.toBinaryData();
}
