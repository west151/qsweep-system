#ifndef MQTT_PROVIDER_H
#define MQTT_PROVIDER_H

#include <QObject>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

#include "sweep_topic.h"

class sweep_write_settings;

class mqtt_provider : public QObject
{
    Q_OBJECT
public:
    explicit mqtt_provider(QObject *parent = nullptr);

    void initialization();
    void set_configuration(const sweep_write_settings &);

    void launching();
    void stopping();

signals:
    void signal_state_connected();
    void signal_state_disconnected();

private slots:
    void slot_publish_message(const QByteArray &);
    void slot_message_received(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());

private:
    QMqttClient* ptr_mqtt_client {Q_NULLPTR};
    sweep_topic* ptr_sweep_topic {Q_NULLPTR};
    sweep_write_settings *ptr_sweep_write_settings {Q_NULLPTR};

    QList<sweep_topic::topic> m_subscribe_topic;

    void update_state_change();
    void broker_disconnected();
    void ping_received();
    void connecting();

    void unsubscribe_broker();
    void subscribe_broker();

    void error_changed(QMqttClient::ClientError error);
};

#endif // MQTT_PROVIDER_H
