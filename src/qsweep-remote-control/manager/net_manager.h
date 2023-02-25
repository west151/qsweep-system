#ifndef NET_MANAGER_H
#define NET_MANAGER_H

#include <QObject>

class mqtt_provider_settings;
class mqtt_provider;
class sweep_topic;

class net_manager : public QObject
{
    Q_OBJECT
public:
    explicit net_manager(QObject *parent = nullptr);

    void set_settings(const QByteArray &);

signals:
    void signal_received_sdr_info(const QByteArray &);

public slots:
    void slot_start();
    void slot_publish_ctrl(const QByteArray &);

private:
    sweep_topic* ptr_sweep_topic {Q_NULLPTR};
    mqtt_provider* ptr_mqtt_provider {Q_NULLPTR};
    mqtt_provider_settings* ptr_mqtt_provider_settings {Q_NULLPTR};
};

#endif // NET_MANAGER_H
