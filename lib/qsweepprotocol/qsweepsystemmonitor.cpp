#include "qsweepsystemmonitor.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QDateTime>

static const QString DT_FORMAT = QStringLiteral("hh:mm:ss.zzz dd.MM.yyyy");
static const QString HOST_NAME_KEY = QStringLiteral("hostname");
static const QString UPTIME_KEY = QStringLiteral("uptime");
static const QString CPU_ARCHITECTURE_KEY = QStringLiteral("cpu_arch");
static const QString DT_KEY = QStringLiteral("dt");
static const QString TOTAL_MEMORY_KEY = QStringLiteral("totalmem");
static const QString FREE_MEMORY_KEY = QStringLiteral("freememory");
static const QString BUFFER_MEMORY_KEY = QStringLiteral("buffermem");

class QSweepSystemMonitorData : public QSharedData {
public:
    QSweepSystemMonitorData(): QSharedData()
    {
        m_valid = false;
        m_hostName.clear();
        m_uptime.clear();
        m_currentCpuArchitecture.clear();
        m_totalMemory = -1;
        m_freeMemory = -1;
        m_bufferMemory = -1;
        m_dateTime = QDateTime::currentDateTimeUtc();
    }
    QSweepSystemMonitorData(const QSweepSystemMonitorData &other) : QSharedData(other)
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

    ~QSweepSystemMonitorData() {}

    bool m_valid;
    QString m_hostName;
    QString m_uptime;
    QString m_currentCpuArchitecture;
    int m_totalMemory;
    int m_freeMemory;
    int m_bufferMemory;
    QDateTime m_dateTime;
};

QSweepSystemMonitor::QSweepSystemMonitor() : data(new QSweepSystemMonitorData)
{
}

QSweepSystemMonitor::QSweepSystemMonitor(const QSweepSystemMonitor &rhs) : data(rhs.data)
{
}

QSweepSystemMonitor::QSweepSystemMonitor(const QByteArray &json, const bool binary) : data(new QSweepSystemMonitorData)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    QJsonObject jsonObject = doc.object();
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

QSweepSystemMonitor &QSweepSystemMonitor::operator=(const QSweepSystemMonitor &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

QSweepSystemMonitor::~QSweepSystemMonitor()
{
}

bool QSweepSystemMonitor::isValid() const
{
    return data->m_valid;
}

QString QSweepSystemMonitor::hostName() const
{
    return data->m_hostName;
}

void QSweepSystemMonitor::setHostName(const QString &value)
{
    data->m_hostName = value;
}

QString QSweepSystemMonitor::uptime() const
{
    return  data->m_uptime;
}

void QSweepSystemMonitor::setUptime(const QString &value)
{
    data->m_uptime = value;
}

QString QSweepSystemMonitor::currentCpuArchitecture() const
{
    return data->m_currentCpuArchitecture;
}

void QSweepSystemMonitor::setCurrentCpuArchitecture(const QString &value)
{
    data->m_currentCpuArchitecture = value;
}

int QSweepSystemMonitor::totalMemory() const
{
    return  data->m_totalMemory;
}

void QSweepSystemMonitor::setTotalMemory(const int &value)
{
    data->m_totalMemory = value;
}

int QSweepSystemMonitor::freeMemory() const
{
    return  data->m_freeMemory;
}

void QSweepSystemMonitor::setFreeMemory(const int &value)
{
    data->m_freeMemory = value;
}

int QSweepSystemMonitor::bufferMemory() const
{
    return  data->m_bufferMemory;
}

void QSweepSystemMonitor::setBufferMemory(const int &value)
{
    data->m_bufferMemory = value;
}

QDateTime QSweepSystemMonitor::dateTime() const
{
    return data->m_dateTime;
}

QByteArray QSweepSystemMonitor::exportToJson(const bool binary) const
{
    QJsonObject jsonObject;
    jsonObject.insert(HOST_NAME_KEY, data->m_hostName);
    jsonObject.insert(UPTIME_KEY, data->m_uptime);
    jsonObject.insert(DT_KEY, data->m_dateTime.toUTC().toString(DT_FORMAT));
    jsonObject.insert(CPU_ARCHITECTURE_KEY, data->m_currentCpuArchitecture);
    jsonObject.insert(TOTAL_MEMORY_KEY, data->m_totalMemory);
    jsonObject.insert(FREE_MEMORY_KEY, data->m_freeMemory);
    jsonObject.insert(BUFFER_MEMORY_KEY, data->m_bufferMemory);

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
