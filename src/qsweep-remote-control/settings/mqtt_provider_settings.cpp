#include "mqtt_provider_settings.h"

#include <QJsonObject>
#include <QJsonDocument>

static const QString HOST_BROKER_KEY = QStringLiteral("host_broker");
static const QString PORT_BROKER_KEY = QStringLiteral("port_broker");
static const QString CLIENT_ID_KEY = QStringLiteral("client_id");

class mqtt_provider_settings_data : public QSharedData {
public:
    mqtt_provider_settings_data(): QSharedData()
    {
        m_valid = false;
        m_host_broker = "127.0.0.1";
        m_port_broker = 1883;
        m_client_id.clear();
    }
    mqtt_provider_settings_data(const mqtt_provider_settings_data &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_host_broker = other.m_host_broker;
        m_port_broker = other.m_port_broker;
        m_client_id = other.m_client_id;
    }

    ~mqtt_provider_settings_data() {}

    bool m_valid;
    // broker
    quint16 m_port_broker;
    QString m_host_broker;
    QString m_client_id;
};

mqtt_provider_settings::mqtt_provider_settings() : data(new mqtt_provider_settings_data)
{
}

mqtt_provider_settings::mqtt_provider_settings(const mqtt_provider_settings &rhs) : data(rhs.data)
{
}

mqtt_provider_settings::mqtt_provider_settings(const QByteArray &json) : data(new mqtt_provider_settings_data)
{
    QJsonDocument doc;

    doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object = doc.object();
    data->m_host_broker = json_object.value(HOST_BROKER_KEY).toString();
    data->m_port_broker = json_object.value(PORT_BROKER_KEY).toString().toUShort();
    data->m_client_id = json_object.value(CLIENT_ID_KEY).toString();

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

mqtt_provider_settings &mqtt_provider_settings::operator=(const mqtt_provider_settings &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

mqtt_provider_settings::~mqtt_provider_settings()
{
}

void mqtt_provider_settings::set_settings(const mqtt_provider_settings &settings)
{
    data->m_valid = settings.is_valid();
    data->m_host_broker = settings.host_broker();
    data->m_port_broker = settings.port_broker();
    data->m_client_id = settings.client_id();
}

bool mqtt_provider_settings::is_valid() const
{
    return data->m_valid;
}

void mqtt_provider_settings::set_host_broker(const QString &value)
{
    data->m_host_broker = value;
}

QString mqtt_provider_settings::host_broker() const
{
    return data->m_host_broker;
}

void mqtt_provider_settings::set_client_id(const QString &value)
{
    data->m_client_id = value;
}

QString mqtt_provider_settings::client_id() const
{
    return data->m_client_id;
}

void mqtt_provider_settings::set_port_broker(const quint16 &value)
{
    data->m_port_broker = value;
}

quint16 mqtt_provider_settings::port_broker() const
{
    return data->m_port_broker;
}

QByteArray mqtt_provider_settings::to_json() const
{
    QJsonObject json_object;
    json_object.insert(HOST_BROKER_KEY, data->m_host_broker);
    json_object.insert(PORT_BROKER_KEY, QString::number(data->m_port_broker));
    json_object.insert(CLIENT_ID_KEY, data->m_client_id);

    QJsonDocument doc(json_object);

    return doc.toJson();
}
