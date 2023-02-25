#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class system_monitor_data;

class system_monitor
{
public:
    system_monitor();
    system_monitor(const system_monitor &);
    system_monitor(const QByteArray &json);
    system_monitor &operator=(const system_monitor &);
    ~system_monitor();

    bool is_valid() const;

    QString host_name()const;
    void set_host_name(const QString &);

    QString uptime()const;
    void set_uptime(const QString &);

    QString current_cpu_architecture()const;
    void set_current_cpu_architecture(const QString &);

    int total_memory()const;
    void set_total_memory(const int &);

    int free_memory()const;
    void set_free_memory(const int &);

    int buffer_memory()const;
    void set_buffer_memory(const int &);

    QDateTime date_time()const;

    QByteArray to_json() const;

private:
    QSharedDataPointer<system_monitor_data> data;
};

Q_DECLARE_METATYPE(system_monitor)

#endif // SYSTEM_MONITOR_H
