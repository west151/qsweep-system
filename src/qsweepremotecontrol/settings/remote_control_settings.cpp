#include "remote_control_settings.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

static const QString HOST_BROKER_KEY = QStringLiteral("host_broker");
static const QString PORT_BROKER_KEY = QStringLiteral("port_broker");

class remote_control_settings_data : public QSharedData {
public:
    remote_control_settings_data(): QSharedData()
    {
        m_valid = false;
        m_host_broker = "127.0.0.1";
        m_port_broker = 1883;
    }
    remote_control_settings_data(const remote_control_settings_data &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_host_broker = other.m_host_broker;
        m_port_broker = other.m_port_broker;
    }

    ~remote_control_settings_data() {}

    bool m_valid;
    // broker
    quint16 m_port_broker;
    QString m_host_broker;
};

remote_control_settings::remote_control_settings() : data(new remote_control_settings_data)
{
}

remote_control_settings::remote_control_settings(const remote_control_settings &rhs) : data(rhs.data)
{
}

remote_control_settings::remote_control_settings(const QByteArray &json, const bool binary) : data(new remote_control_settings_data)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object = doc.object();
    data->m_host_broker = json_object.value(HOST_BROKER_KEY).toString();
    data->m_port_broker = json_object.value(PORT_BROKER_KEY).toString().toUShort();

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

remote_control_settings &remote_control_settings::operator=(const remote_control_settings &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

remote_control_settings::~remote_control_settings()
{
}

void remote_control_settings::set_settings(const remote_control_settings &settings)
{
    data->m_valid = settings.is_valid();
    data->m_host_broker = settings.host_broker();
    data->m_port_broker = settings.port_broker();
}

bool remote_control_settings::is_valid() const
{
    return data->m_valid;
}

void remote_control_settings::set_host_broker(const QString &value)
{
    data->m_host_broker = value;
}

QString remote_control_settings::host_broker() const
{
    return data->m_host_broker;
}

void remote_control_settings::set_port_broker(const quint16 &value)
{
    data->m_port_broker = value;
}

quint16 remote_control_settings::port_broker() const
{
    return data->m_port_broker;
}

QByteArray remote_control_settings::to_json(const bool is_binary, const bool is_compact) const
{
    QJsonObject json_object;
    json_object.insert(HOST_BROKER_KEY, data->m_host_broker);
    json_object.insert(PORT_BROKER_KEY, QString::number(data->m_port_broker));

    QJsonDocument doc(json_object);

    if(is_binary)
        return doc.toBinaryData();
    else{
        if(is_compact)
            return doc.toJson(QJsonDocument::Compact);
        else
            return doc.toJson();
    }
}
