#include "qsweeprequest.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUuid>

#include "constkeys.h"

QSweepRequest::QSweepRequest(QObject *parent) : QObject(parent)
{
    m_id = QUuid::createUuid().toString().mid(1, 8);
    m_valid = false;
    m_typeRequest = TypeRequest::UNKNOWN;
    m_dateTimeRequest = QDateTime::currentDateTimeUtc();
}

QSweepRequest::QSweepRequest(const QByteArray &json, bool binary)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    const QJsonObject jsonObject(doc.object());
    m_id = jsonObject.value(ID_KEY).toString();
    m_typeRequest = static_cast<TypeRequest>(jsonObject.value(TYPE_REQUEST_KEY).toInt(0));
    auto dt = QDateTime::fromString(jsonObject.value(DT_REQUEST_KEY).toString(), DT_FORMAT);
    dt.setTimeSpec(Qt::UTC);
    m_dateTimeRequest = dt;

    QByteArray ba;
    ba.append(jsonObject.value(DATA_KEY).toString());
    m_data = QByteArray::fromBase64(ba);

    if(!doc.isEmpty())
        m_valid = true;
    else
        m_valid = false;
}

bool QSweepRequest::isValid() const
{
    return m_valid;
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

void QSweepRequest::setDateTimeRequest(const QDateTime &value)
{
    m_dateTimeRequest = value;
}

QDateTime QSweepRequest::dateTimeRequest() const
{
    return m_dateTimeRequest;
}

void QSweepRequest::setDataRequest(const QByteArray &json)
{
    m_data = json;
}

QByteArray QSweepRequest::dataRequest() const
{
    return m_data;
}

QByteArray QSweepRequest::exportToJson(bool binary) const
{
    QJsonObject jsonObject;
    jsonObject.insert(ID_KEY, m_id);
    jsonObject.insert(TYPE_REQUEST_KEY, static_cast<qint32>(m_typeRequest));
    jsonObject.insert(DT_REQUEST_KEY, m_dateTimeRequest.toUTC().toString(DT_FORMAT));
    jsonObject.insert(DATA_KEY, QString(m_data.toBase64()));

    const QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
