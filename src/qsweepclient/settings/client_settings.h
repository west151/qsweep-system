#ifndef CLIENT_SETTINGS_H
#define CLIENT_SETTINGS_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class sweep_client_settings_data;

class client_settings
{
public:
    client_settings();
    client_settings(const client_settings &);
    client_settings(const QByteArray &json, const bool binary = false);
    client_settings &operator=(const client_settings &);
    ~client_settings();

    bool is_valid() const;

    void set_host_broker(const QString &);
    QString host_broker()const;

    void set_port_broker(const quint16 &);
    quint16 port_broker()const;

    void set_max_size_message_log(const qint32 &);
    qint32 max_size_message_log()const;

    QByteArray to_json(const bool binary = false, const bool isCompact = true) const;

private:
    QSharedDataPointer<sweep_client_settings_data> data;
};

Q_DECLARE_METATYPE(client_settings)

#endif // CLIENT_SETTINGS_H
