#include "sweepclientsettings.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

static const QString HOST_BROKER_KEY = QStringLiteral("host_broker");
static const QString PORT_BROKER_KEY = QStringLiteral("port_broker");
static const QString MAX_SIZE_MESSAGE_LOG_KEY = QStringLiteral("max_size_message_log");

class SweepClientSettingsData : public QSharedData {
public:
    SweepClientSettingsData(): QSharedData()
    {
        m_valid = false;
        m_hostBroker = "127.0.0.1";
        m_portBroker = 1883;
        m_maxSizeMessageLog = 20;
    }
    SweepClientSettingsData(const SweepClientSettingsData &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_hostBroker = other.m_hostBroker;
        m_portBroker = other.m_portBroker;
        m_maxSizeMessageLog = other.m_maxSizeMessageLog;
    }

    ~SweepClientSettingsData() {}

    bool m_valid;
    QString m_hostBroker;
    quint16 m_portBroker;
    qint32 m_maxSizeMessageLog;
};

SweepClientSettings::SweepClientSettings() : data(new SweepClientSettingsData)
{
}

SweepClientSettings::SweepClientSettings(const SweepClientSettings &rhs) : data(rhs.data)
{
}

SweepClientSettings::SweepClientSettings(const QByteArray &json, const bool binary) : data(new SweepClientSettingsData)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    const QJsonObject jsonObject = doc.object();
    data->m_hostBroker = jsonObject.value(HOST_BROKER_KEY).toString();
    data->m_portBroker = jsonObject.value(PORT_BROKER_KEY).toString().toUShort();
    data->m_maxSizeMessageLog = jsonObject.value(MAX_SIZE_MESSAGE_LOG_KEY).toInt(5);

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

SweepClientSettings &SweepClientSettings::operator=(const SweepClientSettings &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

SweepClientSettings::~SweepClientSettings()
{
}

bool SweepClientSettings::isValid() const
{
    return data->m_valid;
}

void SweepClientSettings::setHostBroker(const QString &value)
{
    data->m_hostBroker = value;
}

QString SweepClientSettings::hostBroker() const
{
    return data->m_hostBroker;
}

void SweepClientSettings::setPortBroker(const quint16 &value)
{
    data->m_portBroker = value;
}

quint16 SweepClientSettings::portBroker() const
{
    return data->m_portBroker;
}

void SweepClientSettings::setMaxSizeMessageLog(const qint32 &value)
{
    data->m_maxSizeMessageLog = value;
}

qint32 SweepClientSettings::maxSizeMessageLog() const
{
    return  data->m_maxSizeMessageLog;
}

QByteArray SweepClientSettings::exportToJson(const bool binary) const
{
    QJsonObject jsonObject;
    jsonObject.insert(HOST_BROKER_KEY, data->m_hostBroker);
    jsonObject.insert(PORT_BROKER_KEY, QString::number(data->m_portBroker));
    jsonObject.insert(MAX_SIZE_MESSAGE_LOG_KEY, data->m_maxSizeMessageLog);

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
