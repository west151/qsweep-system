#include "qsweepmessagelog.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QDateTime>

static const QString DT_FORMAT = QStringLiteral("hh:mm:ss.zzz dd.MM.yyyy");
static const QString TEXT_MESSAGE_KEY = QStringLiteral("text_msg");
static const QString DT_KEY = QStringLiteral("dt");

class QSweepMessageLogData : public QSharedData {
public:
    QSweepMessageLogData(): QSharedData()
    {
        m_valid = false;
        m_textMessage.clear();
        m_dateTime = QDateTime::currentDateTimeUtc();
    }
    QSweepMessageLogData(const QSweepMessageLogData &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_textMessage = other.m_textMessage;
        m_dateTime = other.m_dateTime;
    }

    ~QSweepMessageLogData() {}

    bool m_valid;
    QString m_textMessage;
    QDateTime m_dateTime;
};

QSweepMessageLog::QSweepMessageLog() : data(new QSweepMessageLogData)
{
}

QSweepMessageLog::QSweepMessageLog(const QSweepMessageLog &rhs) : data(rhs.data)
{
}

QSweepMessageLog::QSweepMessageLog(const QByteArray &json, const bool binary) : data(new QSweepMessageLogData)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    QJsonObject jsonObject = doc.object();
    data->m_textMessage = jsonObject.value(TEXT_MESSAGE_KEY).toString();
    auto dt = QDateTime::fromString(jsonObject.value(DT_KEY).toString(), DT_FORMAT);
    dt.setTimeSpec(Qt::UTC);
    data->m_dateTime = dt;

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

QSweepMessageLog &QSweepMessageLog::operator=(const QSweepMessageLog &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

QSweepMessageLog::~QSweepMessageLog()
{
}

bool QSweepMessageLog::isValid() const
{
    return data->m_valid;
}

QString QSweepMessageLog::textMessage() const
{
    return data->m_textMessage;
}

void QSweepMessageLog::setTextMessage(const QString &value)
{
    data->m_textMessage = value;
}

QDateTime QSweepMessageLog::dateTime() const
{
    return data->m_dateTime;
}

QByteArray QSweepMessageLog::exportToJson(const bool binary) const
{
    QJsonObject jsonObject;
    jsonObject.insert(TEXT_MESSAGE_KEY, data->m_textMessage);
    jsonObject.insert(DT_KEY, data->m_dateTime.toUTC().toString(DT_FORMAT));

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
