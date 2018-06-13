#include "qsweepanswer.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUuid>

static const QString DT_FORMAT = QStringLiteral("hh:mm dd.MM.yyyy");
static const QString ID_KEY = QStringLiteral("id");
static const QString TYPE_ANSWER_KEY = QStringLiteral("TypeAnswer");
static const QString DT_ANSWER_KEY = QStringLiteral("DateTimeAnswer");

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
    m_id = jsonObject[ID_KEY].toString();
    m_typeAnswer = static_cast<TypeAnswer>(jsonObject[TYPE_ANSWER_KEY].toInt(0));
    auto dt = QDateTime::fromString(jsonObject[DT_ANSWER_KEY].toString(), DT_FORMAT);
    dt.setTimeSpec(Qt::UTC);
    m_dateTimeAnswer = dt;

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

QByteArray QSweepAnswer::exportToJson() const
{
    QJsonObject jsonObject;
    jsonObject[ID_KEY] = m_id;
    jsonObject[TYPE_ANSWER_KEY] = static_cast<qint32>(m_typeAnswer);
    jsonObject[DT_ANSWER_KEY] = m_dateTimeAnswer.toUTC().toString(DT_FORMAT);

    QJsonDocument doc(jsonObject);

    return doc.toJson(QJsonDocument::Compact);
}

QByteArray QSweepAnswer::exportToJsonBinary() const
{
    QJsonObject jsonObject;
    jsonObject[ID_KEY] = m_id;
    jsonObject[TYPE_ANSWER_KEY] = static_cast<qint32>(m_typeAnswer);
    jsonObject[DT_ANSWER_KEY] = m_dateTimeAnswer.toUTC().toString(DT_FORMAT);

    QJsonDocument doc(jsonObject);

    return doc.toBinaryData();
}
