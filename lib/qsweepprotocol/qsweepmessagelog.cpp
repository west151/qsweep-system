#include "qsweepmessagelog.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

static const QString DT_FORMAT = QStringLiteral("hh:mm dd.MM.yyyy");
static const QString TEXT_MESSAGE_KEY = QStringLiteral("text_msg");
static const QString DT_KEY = QStringLiteral("dt");

QSweepMessageLog::QSweepMessageLog(QObject *parent) : QObject(parent)
{
    m_valid = false;
    m_dateTime = QDateTime::currentDateTimeUtc();
    m_textMessage.clear();
}

QSweepMessageLog::QSweepMessageLog(const QByteArray &json, bool binary)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    QJsonObject jsonObject = doc.object();
    m_textMessage = jsonObject[TEXT_MESSAGE_KEY].toString();
    auto dt = QDateTime::fromString(jsonObject[DT_KEY].toString(), DT_FORMAT);
    dt.setTimeSpec(Qt::UTC);
    m_dateTime = dt;

    if(!doc.isEmpty())
        m_valid = true;
    else
        m_valid = false;
}

bool QSweepMessageLog::isValid() const
{
    return m_valid;
}

QDateTime QSweepMessageLog::dateTime() const
{
    return m_dateTime;
}

QString QSweepMessageLog::textMessage() const
{
    return m_textMessage;
}

void QSweepMessageLog::setTextMessage(const QString &value)
{
    m_textMessage = value;
}

QByteArray QSweepMessageLog::exportToJson(bool binary) const
{
    QJsonObject jsonObject;
    jsonObject[TEXT_MESSAGE_KEY] = m_textMessage;

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
