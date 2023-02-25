#include "remote_control_settings.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

#include "mqtt_provider_settings.h"

static const QString MQTT_PROVIDER_KEY = QStringLiteral("mqtt_provider");

class remote_control_settings_data : public QSharedData {
public:
    remote_control_settings_data(): QSharedData()
    {
        m_valid = false;
    }
    remote_control_settings_data(const remote_control_settings_data &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_mqtt_provider_settings = other.m_mqtt_provider_settings;
    }

    ~remote_control_settings_data() {}

    bool m_valid;
    // broker
    mqtt_provider_settings m_mqtt_provider_settings;

};

remote_control_settings::remote_control_settings() : data(new remote_control_settings_data)
{
}

remote_control_settings::remote_control_settings(const remote_control_settings &rhs) : data(rhs.data)
{
}

remote_control_settings::remote_control_settings(const QByteArray &json) : data(new remote_control_settings_data)
{
    QJsonDocument doc;

    doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object = doc.object();

    //mqtt_provider_settings tt(json_object.value(MQTT_PROVIDER_KEY).toString());
    //data->m_host_broker = json_object.value(HOST_BROKER_KEY).toString();
    //data->m_port_broker = json_object.value(PORT_BROKER_KEY).toString().toUShort();

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

void remote_control_settings::set_settings(const remote_control_settings &value)
{
    data->m_valid = value.is_valid();
    data->m_mqtt_provider_settings = value.mqtt_provider_settings_data();
}

void remote_control_settings::set_mqtt_provider_settings_data(const mqtt_provider_settings &value)
{
    data->m_mqtt_provider_settings = value;
}

mqtt_provider_settings remote_control_settings::mqtt_provider_settings_data() const
{
    return data->m_mqtt_provider_settings;
}

bool remote_control_settings::is_valid() const
{
    return data->m_valid;
}

QByteArray remote_control_settings::to_json() const
{
    QJsonObject json_object;
    json_object.insert(MQTT_PROVIDER_KEY, QString(data->m_mqtt_provider_settings.to_json()));

    QJsonDocument doc(json_object);

    return doc.toJson();
}
