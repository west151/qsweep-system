#include "systemmonitorinterface.h"

SystemMonitorInterface::SystemMonitorInterface(QObject *parent) : QObject(parent)
{

}

QString SystemMonitorInterface::hostName() const
{
    return m_systemMonitor.hostName();
}

QString SystemMonitorInterface::uptime() const
{
    return m_systemMonitor.uptime();
}

int SystemMonitorInterface::totalMemory() const
{
    return m_systemMonitor.totalMemory();
}

int SystemMonitorInterface::freeMemory() const
{
    return  m_systemMonitor.freeMemory();
}

int SystemMonitorInterface::bufferMemory() const
{
    return  m_systemMonitor.bufferMemory();
}

void SystemMonitorInterface::setSystemMonitor(const QSweepSystemMonitor &value)
{
    m_systemMonitor = value;

    emit sendSystemMonitorChanged();
}
