#include "sweepserversettings.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

static const QString HOST_BROKER_KEY = QStringLiteral("host_broker");
static const QString PORT_BROKER_KEY = QStringLiteral("port_broker");
static const QString SYSTEM_MONITOR_INTERVAL_KEY = QStringLiteral("system_monitor_interval");
static const QString DELAYED_LAUNCH_KEY = QStringLiteral("delayed_launch");
static const QString ID_KEY = QStringLiteral("id");

class SweepServerSettingsData : public QSharedData {
public:
    SweepServerSettingsData(): QSharedData()
    {
        m_valid = false;
        m_hostBroker = "127.0.0.1";
        m_portBroker = 1883;
        m_systemMonitorInterval = 1000;
        m_delayedLaunch = 1000;
        m_id = "unknow";
    }
    SweepServerSettingsData(const SweepServerSettingsData &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_hostBroker = other.m_hostBroker;
        m_portBroker = other.m_portBroker;
        m_systemMonitorInterval = other.m_systemMonitorInterval;
        m_delayedLaunch = other.m_delayedLaunch;
        m_id = other.m_id;
    }

    ~SweepServerSettingsData() {}

    bool m_valid;
    QString m_hostBroker;
    quint16 m_portBroker;
    int m_systemMonitorInterval;
    int m_delayedLaunch;
    QString m_id;
};

SweepServerSettings::SweepServerSettings() : data(new SweepServerSettingsData)
{
}

SweepServerSettings::SweepServerSettings(const SweepServerSettings &rhs) : data(rhs.data)
{
}

SweepServerSettings::SweepServerSettings(const QByteArray &json, const bool binary) : data(new SweepServerSettingsData)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    const QJsonObject jsonObject = doc.object();
    data->m_hostBroker = jsonObject.value(HOST_BROKER_KEY).toString();
    data->m_portBroker = jsonObject.value(PORT_BROKER_KEY).toString().toUShort();
    data->m_systemMonitorInterval = jsonObject.value(SYSTEM_MONITOR_INTERVAL_KEY).toInt(1000);
    data->m_delayedLaunch = jsonObject.value(DELAYED_LAUNCH_KEY).toInt(1000);
    data->m_id = jsonObject.value(ID_KEY).toString();

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

SweepServerSettings &SweepServerSettings::operator=(const SweepServerSettings &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

SweepServerSettings::~SweepServerSettings()
{
}

bool SweepServerSettings::isValid() const
{
    return data->m_valid;
}

void SweepServerSettings::setHostBroker(const QString &value)
{
    data->m_hostBroker = value;
}

QString SweepServerSettings::hostBroker() const
{
    return data->m_hostBroker;
}

void SweepServerSettings::setPortBroker(const quint16 &value)
{
    data->m_portBroker = value;
}

quint16 SweepServerSettings::portBroker() const
{
    return data->m_portBroker;
}

void SweepServerSettings::setSystemMonitorInterval(const int &value)
{
    data->m_systemMonitorInterval = value;
}

int SweepServerSettings::systemMonitorInterval() const
{
    return data->m_systemMonitorInterval;
}

void SweepServerSettings::setDelayedLaunch(const int &value)
{
    data->m_delayedLaunch = value;
}

int SweepServerSettings::delayedLaunch() const
{
    return  data->m_delayedLaunch;
}

void SweepServerSettings::setId(const QString &value)
{
    data->m_id = value;
}

QString SweepServerSettings::id() const
{
    return  data->m_id;
}

QByteArray SweepServerSettings::exportToJson(const bool binary) const
{
    QJsonObject jsonObject;
    jsonObject.insert(HOST_BROKER_KEY, data->m_hostBroker);
    jsonObject.insert(PORT_BROKER_KEY, QString::number(data->m_portBroker));
    jsonObject.insert(SYSTEM_MONITOR_INTERVAL_KEY, data->m_systemMonitorInterval);
    jsonObject.insert(DELAYED_LAUNCH_KEY, data->m_delayedLaunch);
    jsonObject.insert(ID_KEY, data->m_id);

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
