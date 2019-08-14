#include "server_settings.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

static const QString HOST_BROKER_KEY = QStringLiteral("host_broker");
static const QString PORT_BROKER_KEY = QStringLiteral("port_broker");
static const QString SYSTEM_MONITOR_INTERVAL_KEY = QStringLiteral("system_monitor_interval");
static const QString DELAYED_LAUNCH_KEY = QStringLiteral("delayed_launch");
static const QString ID_KEY = QStringLiteral("id");
static const QString SPECTRUM_NATIVE_KEY = QStringLiteral("spectrum_source_native");
static const QString SPECTRUM_PROCESS_NAME_KEY = QStringLiteral("spectrum_process_name");

class server_settings_data : public QSharedData {
public:
    server_settings_data(): QSharedData()
    {
        valid = false;
        host_broker = "127.0.0.1";
        port_broker = 1883;
        system_monitor_interval = 1000;
        delayed_launch = 1000;
        id = "unknow";
        spectrum_source_native = true;
        spectrum_process_name.clear();
    }
    server_settings_data(const server_settings_data &other) : QSharedData(other)
    {
        valid = other.valid;
        host_broker = other.host_broker;
        port_broker = other.port_broker;
        system_monitor_interval = other.system_monitor_interval;
        delayed_launch = other.delayed_launch;
        id = other.id;
        spectrum_source_native = other.spectrum_source_native;
        spectrum_process_name = other.spectrum_process_name;
    }

    ~server_settings_data() {}

    bool valid;
    QString host_broker;
    quint16 port_broker;
    int system_monitor_interval;
    int delayed_launch;
    QString id;
    bool spectrum_source_native;
    QString spectrum_process_name;
};

server_settings::server_settings() : data(new server_settings_data)
{
}

server_settings::server_settings(const server_settings &rhs) : data(rhs.data)
{
}

server_settings::server_settings(const QByteArray &json, const bool binary) : data(new server_settings_data)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object = doc.object();
    data->host_broker = json_object.value(HOST_BROKER_KEY).toString();
    data->port_broker = json_object.value(PORT_BROKER_KEY).toString().toUShort();
    data->system_monitor_interval = json_object.value(SYSTEM_MONITOR_INTERVAL_KEY).toInt(1000);
    data->delayed_launch = json_object.value(DELAYED_LAUNCH_KEY).toInt(1000);
    data->id = json_object.value(ID_KEY).toString();
    data->spectrum_source_native = json_object.value(SPECTRUM_NATIVE_KEY).toBool();
    data->spectrum_process_name = json_object.value(SPECTRUM_PROCESS_NAME_KEY).toString();

    if(!doc.isEmpty())
        data->valid = true;
    else
        data->valid = false;
}

server_settings &server_settings::operator=(const server_settings &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

server_settings::~server_settings()
{
}

bool server_settings::is_valid() const
{
    return data->valid;
}

void server_settings::set_host_broker(const QString &value)
{
    data->host_broker = value;
}

QString server_settings::host_broker() const
{
    return data->host_broker;
}

void server_settings::set_port_broker(const quint16 &value)
{
    data->port_broker = value;
}

quint16 server_settings::port_broker() const
{
    return data->port_broker;
}

void server_settings::set_system_monitor_interval(const int &value)
{
    data->system_monitor_interval = value;
}

int server_settings::system_monitor_interval() const
{
    return data->system_monitor_interval;
}

void server_settings::set_delayed_launch(const int &value)
{
    data->delayed_launch = value;
}

int server_settings::delayed_launch() const
{
    return  data->delayed_launch;
}

void server_settings::set_spectrum_source_native(const bool &value)
{
    data->spectrum_source_native = value;
}

bool server_settings::spectrum_source_native() const
{
    return data->spectrum_source_native;
}

void server_settings::set_spectrum_process_name(const QString &value)
{
    data->spectrum_process_name = value;
}

QString server_settings::spectrum_process_name() const
{
    return data->spectrum_process_name;
}

void server_settings::set_id(const QString &value)
{
    data->id = value;
}

QString server_settings::id() const
{
    return  data->id;
}

QByteArray server_settings::to_json(const bool binary, const bool isCompact) const
{
    QJsonObject json_object;
    json_object.insert(HOST_BROKER_KEY, data->host_broker);
    json_object.insert(PORT_BROKER_KEY, QString::number(data->port_broker));
    json_object.insert(SYSTEM_MONITOR_INTERVAL_KEY, data->system_monitor_interval);
    json_object.insert(DELAYED_LAUNCH_KEY, data->delayed_launch);
    json_object.insert(ID_KEY, data->id);
    json_object.insert(SPECTRUM_NATIVE_KEY, data->spectrum_source_native);
    json_object.insert(SPECTRUM_PROCESS_NAME_KEY, data->spectrum_process_name);

    QJsonDocument doc(json_object);

    if(binary)
        return doc.toBinaryData();
    else{
        if(isCompact)
            return doc.toJson(QJsonDocument::Compact);
        else
            return doc.toJson();
    }
}
