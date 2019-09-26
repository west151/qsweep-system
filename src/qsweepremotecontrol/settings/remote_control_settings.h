#ifndef REMOTE_CONTROL_SETTINGS_H
#define REMOTE_CONTROL_SETTINGS_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class remote_control_settings_data;

class remote_control_settings
{
public:
    remote_control_settings();
    remote_control_settings(const remote_control_settings &);
    remote_control_settings(const QByteArray &json, const bool binary = false);
    remote_control_settings &operator=(const remote_control_settings &);
    ~remote_control_settings();

    void set_settings(const remote_control_settings &);

    bool is_valid() const;

    void set_host_broker(const QString &);
    QString host_broker()const;

    void set_port_broker(const quint16 &);
    quint16 port_broker()const;

    QByteArray to_json(const bool is_binary = false, const bool is_compact = true) const;

private:
    QSharedDataPointer<remote_control_settings_data> data;
};

Q_DECLARE_METATYPE(remote_control_settings)

#endif // REMOTE_CONTROL_SETTINGS_H
