#ifndef MQTT_PROVIDER_SETTINGS_H
#define MQTT_PROVIDER_SETTINGS_H

#include <QSharedData>
#include <QMetaType>

class mqtt_provider_settings_data;

class mqtt_provider_settings
{
public:
    mqtt_provider_settings();
    mqtt_provider_settings(const mqtt_provider_settings &);
    mqtt_provider_settings(const QByteArray &json);
    mqtt_provider_settings &operator=(const mqtt_provider_settings &);
    ~mqtt_provider_settings();

    void set_settings(const mqtt_provider_settings &);

    bool is_valid() const;

    void set_host_broker(const QString &);
    QString host_broker()const;

    void set_client_id(const QString &);
    QString client_id()const;

    void set_port_broker(const quint16 &);
    quint16 port_broker()const;

    QByteArray to_json() const;

private:
    QSharedDataPointer<mqtt_provider_settings_data> data;
};

#endif // MQTT_PROVIDER_SETTINGS_H

Q_DECLARE_METATYPE(mqtt_provider_settings)
