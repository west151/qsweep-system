#ifndef SWEEPSERVERSETTINGS_H
#define SWEEPSERVERSETTINGS_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class server_settings_data;

class server_settings
{
public:
    server_settings();
    server_settings(const server_settings &);
    server_settings(const QByteArray &json);
    server_settings &operator=(const server_settings &);
    ~server_settings();

    bool is_valid() const;

    void set_host_broker(const QString &);
    QString host_broker()const;

    void set_port_broker(const quint16 &);
    quint16 port_broker()const;

    void set_system_monitor_interval(const int &);
    int system_monitor_interval()const;

    void set_delayed_launch(const int &);
    int delayed_launch()const;

    void set_spectrum_source_native(const bool &);
    bool spectrum_source_native()const;

    void set_spectrum_process_name(const QString &);
    QString spectrum_process_name()const;

    void set_id(const QString &);
    QString id()const;

    QByteArray to_json() const;

private:
    QSharedDataPointer<server_settings_data> data;
};

Q_DECLARE_METATYPE(server_settings)

#endif // SWEEPSERVERSETTINGS_H
