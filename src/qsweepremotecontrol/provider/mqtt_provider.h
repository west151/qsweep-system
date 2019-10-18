#ifndef MQTT_PROVIDER_H
#define MQTT_PROVIDER_H

#include <QObject>
#include <QtMqtt/QMqttClient>

class mqtt_provider : public QObject
{
    Q_OBJECT
public:
    explicit mqtt_provider(QObject *parent = nullptr);

    void set_host_broker(const QString &);
    QString host_broker()const;

    void set_port_broker(const quint16 &);
    quint16 port_broker()const;

    void set_client_id(const QString &);
    QString client_id()const;

    void launching();
    void stopping();

    bool is_ready();

signals:
    void signal_state_connected();
    void signal_state_disconnected();
    void signal_received_message(const QByteArray &, const QString &);

public slots:
    void slot_publish_message(const QByteArray &, const QString &);
    void slot_unsubscribe_broker(const QStringList &);
    void slot_subscribe_broker(const QStringList &);
    void slot_state_connected();

private:
    QString m_client_id;
    QString m_host_broker;
    quint16 m_port_broker;
    QMqttClient* ptr_mqtt_client {Q_NULLPTR};
    QStringList m_list_of_subscribe;

    enum init_state: qint32 {
        param_client_id,
        param_host_broker,
        param_port_broker,
        provider_is_ready
    };

    QHash<init_state, bool> m_state_init_hash;
    bool is_init_state();

    void error_subscription(const QMqttSubscription::SubscriptionState &);

private slots:
    void slot_error_changed(QMqttClient::ClientError error);
    void slot_received_message(const QByteArray &, const QMqttTopicName &topic = QMqttTopicName());
    void slot_update_state_change();
};

#endif // MQTT_PROVIDER_H
