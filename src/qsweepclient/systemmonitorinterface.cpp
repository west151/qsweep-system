#include "systemmonitorinterface.h"

SystemMonitorInterface::SystemMonitorInterface(QObject *parent) : QObject(parent)
{

}

QString SystemMonitorInterface::hostName() const
{
    return m_systemMonitor.host_name();
}

QString SystemMonitorInterface::uptime() const
{
    return m_systemMonitor.uptime();
}

int SystemMonitorInterface::totalMemory() const
{
    return m_systemMonitor.total_memory();
}

int SystemMonitorInterface::freeMemory() const
{
    return  m_systemMonitor.free_memory();
}

int SystemMonitorInterface::bufferMemory() const
{
    return  m_systemMonitor.buffer_memory();
}

void SystemMonitorInterface::slot_system_monitor(const system_monitor &value)
{
    m_systemMonitor = value;

    emit sendSystemMonitorChanged();
}
