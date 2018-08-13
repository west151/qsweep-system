#include "systemmonitorworker.h"

#include <QSysInfo>
#include <sys/sysinfo.h>

#include "qsweepanswer.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

SystemMonitorWorker::SystemMonitorWorker(QObject *parent) : QObject(parent)
{

}

void SystemMonitorWorker::runSystemMonitorWorker()
{
    QSweepAnswer answer;
    answer.setTypeAnswer(TypeAnswer::SWEEP_SYSTEM_MONITOR);
    QSweepSystemMonitor result;

    result.setHostName(QSysInfo::machineHostName());    // Hostname
    result.setCurrentCpuArchitecture(QSysInfo::currentCpuArchitecture()); // Процессор (архитектура)

    struct sysinfo o;
    sysinfo(&o);
    long up = o.uptime;
    int hour = static_cast<int>(up/60/60);
    int min = static_cast<int>((up - hour*60*60) / 60);
    int sec = static_cast<int>((up - hour*60*60) - min*60);
    QString uptime = QString::number(hour) +  QString(" h. ") + QString::number(min) + QString(" m. ")
            + QString::number(sec) + QString(" s.");

    result.setUptime(uptime);   // Uptime

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

    result.setTotalMemory(total);
    result.setFreeMemory(free);
    result.setBufferMemory(buffer);

    // send data to broker
    answer.setDataAnswer(result.exportToJson());
    emit sendSystemMonitorResult(answer.exportToJson());
}
