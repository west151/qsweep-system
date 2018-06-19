#include "qsweeprequest.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUuid>

static const QString DT_FORMAT = QStringLiteral("hh:mm dd.MM.yyyy");
static const QString ID_KEY = QStringLiteral("id");
static const QString TYPE_REQUEST_KEY = QStringLiteral("TypeRequest");
static const QString DT_REQUEST_KEY = QStringLiteral("DateTimeRequest");
static const QString DATA_KEY = QStringLiteral("data");

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

    QJsonObject jsonObject = doc.object();
    m_id = jsonObject[ID_KEY].toString();
    m_typeRequest = static_cast<TypeRequest>(jsonObject[TYPE_REQUEST_KEY].toInt(0));
    auto dt = QDateTime::fromString(jsonObject[DT_REQUEST_KEY].toString(), DT_FORMAT);
    dt.setTimeSpec(Qt::UTC);
    m_dateTimeRequest = dt;

    QByteArray ba;
    ba.append(jsonObject[DATA_KEY].toString());
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
    jsonObject[ID_KEY] = m_id;
    jsonObject[TYPE_REQUEST_KEY] = static_cast<qint32>(m_typeRequest);
    jsonObject[DT_REQUEST_KEY] = m_dateTimeRequest.toUTC().toString(DT_FORMAT);
    jsonObject[DATA_KEY] = QString(m_data.toBase64());

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
