#include "systemmonitorworker.h"

#include <QSysInfo>
#include <sys/sysinfo.h>

#include "system_monitor.h"
#include "sweep_message.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

SystemMonitorWorker::SystemMonitorWorker(QObject *parent) : QObject(parent)
{

}

void SystemMonitorWorker::runSystemMonitorWorker()
{
    system_monitor monitor_data;
    sweep_message send_data;
    send_data.set_type(type_message::data_system_monitor);

    monitor_data.set_host_name(QSysInfo::machineHostName());    // Hostname
    monitor_data.set_current_cpu_architecture(QSysInfo::currentCpuArchitecture());

    struct sysinfo o;
    sysinfo(&o);
    long up = o.uptime;
    int hour = static_cast<int>(up/60/60);
    int min = static_cast<int>((up - hour*60*60) / 60);
    int sec = static_cast<int>((up - hour*60*60) - min*60);
    QString uptime = QString::number(hour) +  QString(" h. ") + QString::number(min) + QString(" m. ")
            + QString::number(sec) + QString(" s.");

    monitor_data.set_uptime(uptime);    // Uptime

    // memory
    int total, free, buffer, memcache, memfree;
    FILE* info = fopen("/proc/meminfo","r");
    //try to parse the old meminfo format
    if(fscanf (info, "MemTotal: %d kB MemFree: %d kB Buffers: %d kB Cached: %d kB",&total, &memfree, &buffer, &memcache) < 4)
    {
        fclose(info);
        info = fopen("/proc/meminfo","r");
        //parsing the old format failed so we try to parse using the new format
        if(fscanf(info, "MemTotal: %d kB MemFree: %d kB MemAvailable: %d kB Buffers: %d kB Cached: %d kB",&total, &memfree, &free, &buffer, &memcache) < 5)
        {
            //parsing failed overall so return -1 for all values
            total = -1;
            free = -1;
            buffer = -1;
        }else {
            free = memfree + (buffer + memcache);  //everything is fine
        }

    }else{
        //compute available memory
        free = memfree + (buffer + memcache);
    }
    fclose(info);

    monitor_data.set_total_memory(total);
    monitor_data.set_free_memory(free);
    monitor_data.set_buffer_memory(buffer);

    send_data.set_data_message(monitor_data.export_json());

    emit signal_system_monitor_result(send_data.export_json());
}
