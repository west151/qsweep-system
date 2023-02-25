#ifndef REMOTE_CONTROL_SETTINGS_H
#define REMOTE_CONTROL_SETTINGS_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class remote_control_settings_data;
class mqtt_provider_settings;

class remote_control_settings
{
public:
    remote_control_settings();
    remote_control_settings(const remote_control_settings &);
    remote_control_settings(const QByteArray &json);
    remote_control_settings &operator=(const remote_control_settings &);
    ~remote_control_settings();

    void set_settings(const remote_control_settings &);
    // mqtt provider settings
    void set_mqtt_provider_settings_data(const mqtt_provider_settings &);
    mqtt_provider_settings mqtt_provider_settings_data()const;

    bool is_valid() const;

    QByteArray to_json() const;

private:
    QSharedDataPointer<remote_control_settings_data> data;
};

Q_DECLARE_METATYPE(remote_control_settings)

#endif // REMOTE_CONTROL_SETTINGS_H
