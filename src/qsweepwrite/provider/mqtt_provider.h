#ifndef MQTT_PROVIDER_H
#define MQTT_PROVIDER_H

#include <QObject>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

class sweep_topic;

class mqtt_provider : public QObject
{
    Q_OBJECT
public:
    explicit mqtt_provider(QObject *parent = nullptr);

signals:

public slots:

private:
    QMqttClient* ptr_mqtt_client {Q_NULLPTR};
    sweep_topic* ptr_sweep_topic {Q_NULLPTR};
};

#endif // MQTT_PROVIDER_H
