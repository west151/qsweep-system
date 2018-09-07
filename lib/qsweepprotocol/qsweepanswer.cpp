#include "qsweepanswer.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUuid>

static const QString DT_FORMAT = QStringLiteral("hh:mm dd.MM.yyyy");
static const QString ID_KEY = QStringLiteral("id");
static const QString TYPE_ANSWER_KEY = QStringLiteral("TypeAnswer");
static const QString DT_ANSWER_KEY = QStringLiteral("DateTimeAnswer");
static const QString DATA_KEY = QStringLiteral("data");

QSweepAnswer::QSweepAnswer(QObject *parent) : QObject(parent)
{
    m_id = QUuid::createUuid().toString().mid(1, 8);
    m_valid = false;
    m_typeAnswer = TypeAnswer::UNKNOWN;
    m_dateTimeAnswer = QDateTime::currentDateTimeUtc();
}

QSweepAnswer::QSweepAnswer(const QByteArray &json, bool binary)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    QJsonObject jsonObject = doc.object();
    m_id = jsonObject.value(ID_KEY).toString();
    m_typeAnswer = static_cast<TypeAnswer>(jsonObject.value(TYPE_ANSWER_KEY).toInt(0));
    auto dt = QDateTime::fromString(jsonObject.value(DT_ANSWER_KEY).toString(), DT_FORMAT);
    dt.setTimeSpec(Qt::UTC);
    m_dateTimeAnswer = dt;

    QByteArray ba;
    ba.append(jsonObject.value(DATA_KEY).toString());
    m_data = QByteArray::fromBase64(ba);

    if(!doc.isEmpty())
        m_valid = true;
    else
        m_valid = false;
}

bool QSweepAnswer::isValid() const
{
    return m_valid;
}

QString QSweepAnswer::idRequest() const
{
    return m_id;
}

void QSweepAnswer::setTypeAnswer(const TypeAnswer &value)
{
    m_typeAnswer = value;
}

TypeAnswer QSweepAnswer::typeAnswer() const
{
    return m_typeAnswer;
}

void QSweepAnswer::setDateTimeAnswer(const QDateTime &value)
{
    m_dateTimeAnswer = value;
}

QDateTime QSweepAnswer::dateTimeAnswer() const
{
    return m_dateTimeAnswer;
}

void QSweepAnswer::setDataAnswer(const QByteArray &json)
{
    m_data = json;
}

QByteArray QSweepAnswer::dataAnswer() const
{
    return m_data;
}

QByteArray QSweepAnswer::exportToJson(bool binary) const
{
    QJsonObject jsonObject;
    jsonObject.insert(ID_KEY, m_id);
    jsonObject.insert(TYPE_ANSWER_KEY, static_cast<qint32>(m_typeAnswer));
    jsonObject.insert(DT_ANSWER_KEY, m_dateTimeAnswer.toUTC().toString(DT_FORMAT));
    jsonObject.insert(DATA_KEY, QString(m_data.toBase64()));

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
