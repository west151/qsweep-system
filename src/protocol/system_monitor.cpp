#include "system_monitor.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QDateTime>

#include "constkeys.h"

class system_monitor_data : public QSharedData {
public:
    system_monitor_data(): QSharedData()
    {
        m_valid = false;
        m_hostName.clear();
        m_uptime.clear();
        m_currentCpuArchitecture.clear();
        m_totalMemory = 0;
        m_freeMemory = 0;
        m_bufferMemory = 0;
        m_dateTime = QDateTime::currentDateTimeUtc();
    }
    system_monitor_data(const system_monitor_data &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_hostName = other.m_hostName;
        m_dateTime = other.m_dateTime;
        m_uptime = other.m_uptime;
        m_totalMemory = other.m_totalMemory;
        m_freeMemory = other.m_freeMemory;
        m_bufferMemory = other.m_bufferMemory;
        m_currentCpuArchitecture = other.m_currentCpuArchitecture;
    }

    ~system_monitor_data() {}

    bool m_valid;
    QString m_hostName;
    QString m_uptime;
    QString m_currentCpuArchitecture;
    int m_totalMemory;
    int m_freeMemory;
    int m_bufferMemory;
    QDateTime m_dateTime;
};

system_monitor::system_monitor() : data(new system_monitor_data)
{
}

system_monitor::system_monitor(const system_monitor &rhs) : data(rhs.data)
{
}

system_monitor::system_monitor(const QByteArray &json) : data(new system_monitor_data)
{
    QJsonDocument doc;

    doc = QJsonDocument::fromJson(json);

    const QJsonObject jsonObject(doc.object());
    data->m_hostName = jsonObject.value(HOST_NAME_KEY).toString();
    data->m_uptime = jsonObject.value(UPTIME_KEY).toString();
    data->m_currentCpuArchitecture = jsonObject.value(CPU_ARCHITECTURE_KEY).toString();
    data->m_totalMemory = jsonObject.value(TOTAL_MEMORY_KEY).toInt();
    data->m_freeMemory = jsonObject.value(FREE_MEMORY_KEY).toInt();
    data->m_bufferMemory = jsonObject.value(BUFFER_MEMORY_KEY).toInt();
    auto dt = QDateTime::fromString(jsonObject.value(DT_KEY).toString(), DT_FORMAT);
    dt.setTimeSpec(Qt::UTC);
    data->m_dateTime = dt;

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

system_monitor &system_monitor::operator=(const system_monitor &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

system_monitor::~system_monitor()
{
}

bool system_monitor::is_valid() const
{
    return data->m_valid;
}

QString system_monitor::host_name() const
{
    return data->m_hostName;
}

void system_monitor::set_host_name(const QString &value)
{
    data->m_hostName = value;
}

QString system_monitor::uptime() const
{
    return  data->m_uptime;
}

void system_monitor::set_uptime(const QString &value)
{
    data->m_uptime = value;
}

QString system_monitor::current_cpu_architecture() const
{
    return data->m_currentCpuArchitecture;
}

void system_monitor::set_current_cpu_architecture(const QString &value)
{
    data->m_currentCpuArchitecture = value;
}

int system_monitor::total_memory() const
{
    return  data->m_totalMemory;
}

void system_monitor::set_total_memory(const int &value)
{
    data->m_totalMemory = value;
}

int system_monitor::free_memory() const
{
    return  data->m_freeMemory;
}

void system_monitor::set_free_memory(const int &value)
{
    data->m_freeMemory = value;
}

int system_monitor::buffer_memory() const
{
    return  data->m_bufferMemory;
}

void system_monitor::set_buffer_memory(const int &value)
{
    data->m_bufferMemory = value;
}

QDateTime system_monitor::date_time() const
{
    return data->m_dateTime;
}

QByteArray system_monitor::to_json() const
{
    QJsonObject jsonObject;
    jsonObject.insert(HOST_NAME_KEY, data->m_hostName);
    jsonObject.insert(UPTIME_KEY, data->m_uptime);
    jsonObject.insert(DT_KEY, data->m_dateTime.toUTC().toString(DT_FORMAT));
    jsonObject.insert(CPU_ARCHITECTURE_KEY, data->m_currentCpuArchitecture);
    jsonObject.insert(TOTAL_MEMORY_KEY, data->m_totalMemory);
    jsonObject.insert(FREE_MEMORY_KEY, data->m_freeMemory);
    jsonObject.insert(BUFFER_MEMORY_KEY, data->m_bufferMemory);

    const QJsonDocument doc(jsonObject);

    return doc.toJson(QJsonDocument::Compact);
}
