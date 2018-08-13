#include "sweepserversettings.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

static const QString HOST_BROKER_KEY = QStringLiteral("host_broker");
static const QString PORT_BROKER_KEY = QStringLiteral("port_broker");

class SweepServerSettingsData : public QSharedData {
public:
    SweepServerSettingsData(): QSharedData()
    {
        m_valid = false;
        m_hostBroker = "127.0.0.1";
        m_portBroker = 1883;
    }
    SweepServerSettingsData(const SweepServerSettingsData &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_hostBroker = other.m_hostBroker;
        m_portBroker = other.m_portBroker;
    }

    ~SweepServerSettingsData() {}

    bool m_valid;
    QString m_hostBroker;
    quint16 m_portBroker;
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

QByteArray SweepServerSettings::exportToJson(const bool binary) const
{
    QJsonObject jsonObject;
    jsonObject.insert(HOST_BROKER_KEY, data->m_hostBroker);
    jsonObject.insert(PORT_BROKER_KEY, QString::number(data->m_portBroker));

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
