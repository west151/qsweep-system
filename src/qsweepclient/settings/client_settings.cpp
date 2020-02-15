#include "client_settings.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

static const QString HOST_BROKER_KEY = QStringLiteral("host_broker");
static const QString PORT_BROKER_KEY = QStringLiteral("port_broker");
static const QString MAX_SIZE_MESSAGE_LOG_KEY = QStringLiteral("max_size_message_log");

class sweep_client_settings_data : public QSharedData {
public:
    sweep_client_settings_data(): QSharedData()
    {
        m_valid = false;
        m_host_broker = "127.0.0.1";
        m_port_broker = 1883;
        m_max_size_message_log = 20;
    }
    sweep_client_settings_data(const sweep_client_settings_data &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_host_broker = other.m_host_broker;
        m_port_broker = other.m_port_broker;
        m_max_size_message_log = other.m_max_size_message_log;
    }

    ~sweep_client_settings_data() {}

    bool m_valid;
    QString m_host_broker;
    quint16 m_port_broker;
    qint32 m_max_size_message_log;
};

client_settings::client_settings() : data(new sweep_client_settings_data)
{
}

client_settings::client_settings(const client_settings &rhs) : data(rhs.data)
{
}

client_settings::client_settings(const QByteArray &json) : data(new sweep_client_settings_data)
{
    QJsonDocument doc;

    doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object = doc.object();
    data->m_host_broker = json_object.value(HOST_BROKER_KEY).toString();
    data->m_port_broker = json_object.value(PORT_BROKER_KEY).toString().toUShort();
    data->m_max_size_message_log = json_object.value(MAX_SIZE_MESSAGE_LOG_KEY).toInt(5);

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

client_settings &client_settings::operator=(const client_settings &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

client_settings::~client_settings()
{
}

bool client_settings::is_valid() const
{
    return data->m_valid;
}

void client_settings::set_host_broker(const QString &value)
{
    data->m_host_broker = value;
}

QString client_settings::host_broker() const
{
    return data->m_host_broker;
}

void client_settings::set_port_broker(const quint16 &value)
{
    data->m_port_broker = value;
}

quint16 client_settings::port_broker() const
{
    return data->m_port_broker;
}

void client_settings::set_max_size_message_log(const qint32 &value)
{
    data->m_max_size_message_log = value;
}

qint32 client_settings::max_size_message_log() const
{
    return  data->m_max_size_message_log;
}

QByteArray client_settings::to_json() const
{
    QJsonObject json_object;
    json_object.insert(HOST_BROKER_KEY, data->m_host_broker);
    json_object.insert(PORT_BROKER_KEY, QString::number(data->m_port_broker));
    json_object.insert(MAX_SIZE_MESSAGE_LOG_KEY, data->m_max_size_message_log);

    QJsonDocument doc(json_object);

    return doc.toJson();
}
