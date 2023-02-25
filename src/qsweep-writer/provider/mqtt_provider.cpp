#include "mqtt_provider.h"
#include "../sweep_write_settings.h"
#include "sweep_message.h"
#include "broker_ctrl.h"

#include <QDateTime>

mqtt_provider::mqtt_provider(QObject *parent) : QObject(parent)
{
    m_subscribe_data.append(sweep_topic::topic_ctrl);
    m_subscribe_ctrl.append(sweep_topic::topic_db_ctrl);
}

void mqtt_provider::initialization()
{
    ptr_mqtt_client = new QMqttClient(this);

    connect(ptr_mqtt_client, &QMqttClient::messageReceived,
            this, &mqtt_provider::slot_message_received);
    connect(ptr_mqtt_client, &QMqttClient::stateChanged,
            this, &mqtt_provider::update_state_change);
    connect(ptr_mqtt_client, &QMqttClient::disconnected,
            this, &mqtt_provider::broker_disconnected);
    connect(ptr_mqtt_client, &QMqttClient::connected,
            this, &mqtt_provider::ping_received);
    connect(ptr_mqtt_client, &QMqttClient::pingResponseReceived,
            this , &mqtt_provider::ping_received);
    connect(ptr_mqtt_client, &QMqttClient::errorChanged,
            this, &mqtt_provider::error_changed);

    ptr_sweep_topic = new sweep_topic(this);

    connect(this, &mqtt_provider::signal_state_connected,
            this, &mqtt_provider::slot_state_connected);
}

void mqtt_provider::set_configuration(const sweep_write_settings &settings)
{
    ptr_sweep_write_settings = new sweep_write_settings(settings);
}

void mqtt_provider::launching()
{
    if(ptr_sweep_write_settings->is_valid())
    {
#ifdef QT_DEBUG
        qDebug().noquote() << tr("settings->") << tr("host broker:") << ptr_sweep_write_settings->host_broker();
        qDebug().noquote() << tr("settings->") << tr("port broker:") << ptr_sweep_write_settings->port_broker();
        qDebug().noquote() << tr("settings->") << tr("delayed launch:") << ptr_sweep_write_settings->delayed_launch();
        qDebug().noquote() << tr("settings->") << tr("db path:") << ptr_sweep_write_settings->db_path();
#endif

        // connect to mqtt broker
        if(ptr_mqtt_client)
        {
            ptr_mqtt_client->setHostname(ptr_sweep_write_settings->host_broker());
            ptr_mqtt_client->setPort(ptr_sweep_write_settings->port_broker());
            ptr_mqtt_client->setClientId("qsweep_writer");

            if (ptr_mqtt_client->state() == QMqttClient::Disconnected)
                ptr_mqtt_client->connectToHost();
        }
    }
}

void mqtt_provider::stopping()
{
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
        ptr_mqtt_client->disconnectFromHost();
}

void mqtt_provider::slot_publish_message(const QByteArray &)
{

}

void mqtt_provider::slot_message_received(const QByteArray &message, const QMqttTopicName &topic)
{
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_db_ctrl)
    {
        const sweep_message data_received(message);

        if(data_received.is_valid())
        {            
            if(data_received.type() == type_message::ctrl_db)
            {
                const broker_ctrl broker_ctrl_data(data_received.data_message());

                if(broker_ctrl_data.ctrl_type()==broker_ctrl_type::subscribe)
                    subscribe_broker(broker_ctrl_data.topic_list());

                if(broker_ctrl_data.ctrl_type()==broker_ctrl_type::unsubscribe)
                    unsubscribe_broker(broker_ctrl_data.topic_list());

#ifdef QT_DEBUG
                qDebug().noquote() << "broker_ctrl"
                                   << broker_ctrl_data.ctrl_type()
                                   << broker_ctrl_data.topic_list();
#endif
            }
        }
    }

    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_power_spectr)
    {
        const sweep_message data_received(message);

        if(data_received.is_valid())
            if(data_received.type() == type_message::data_spectr)
                emit signal_received_data(message);
    }

    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_ctrl)
    {
        const sweep_message data_received(message);

        if(data_received.is_valid())
            if(data_received.type() == type_message::ctrl_spectr)
                emit signal_received_data(message);
    }
}

void mqtt_provider::slot_state_connected()
{
    if(m_subscribe_ctrl.size()>0)
    {
        for (int i=0; i<m_subscribe_ctrl.size(); ++i)
        {
            auto subscription = ptr_mqtt_client->subscribe(ptr_sweep_topic->sweep_topic_by_type(m_subscribe_ctrl.at(i)));
            if (!subscription)
            {
#ifdef QT_DEBUG
                qDebug() << "Could not subscribe. Is there a valid connection?";
#endif
            }else{
#ifdef QT_DEBUG
                qDebug() << "subscribe:" << ptr_sweep_topic->sweep_topic_by_type(m_subscribe_ctrl.at(i));
#endif
            }
        }
    }

    if(m_subscribe_data.size()>0)
    {
        for (int i=0; i<m_subscribe_data.size(); ++i)
        {
            auto subscription = ptr_mqtt_client->subscribe(ptr_sweep_topic->sweep_topic_by_type(m_subscribe_data.at(i)));
            if (!subscription)
            {
#ifdef QT_DEBUG
                qDebug() << "Could not subscribe. Is there a valid connection?";
#endif
            }else{
#ifdef QT_DEBUG
                qDebug() << "subscribe:" << ptr_sweep_topic->sweep_topic_by_type(m_subscribe_data.at(i));
#endif
            }
        }
    }
}

void mqtt_provider::update_state_change()
{
#ifdef QT_DEBUG
    const QString content = QDateTime::currentDateTime().toString()
            + QLatin1String(": State Change ")
            + QString::number(ptr_mqtt_client->state());

    qDebug().noquote() << content;
#endif

    // Subscribers topic
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
    {
        emit signal_state_connected();
    }

    if (ptr_mqtt_client->state() == QMqttClient::Disconnected)
    {
        emit signal_state_disconnected();
    }
}

void mqtt_provider::broker_disconnected()
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
}

void mqtt_provider::ping_received()
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
}

void mqtt_provider::connecting()
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
}

void mqtt_provider::unsubscribe_broker(const QStringList &list_topic)
{
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
        if(!list_topic.isEmpty())
            for(int i=0; i<list_topic.size(); ++i)
                ptr_mqtt_client->unsubscribe(list_topic.at(i));
}

void mqtt_provider::subscribe_broker(const QStringList &list_topic)
{
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
        if(!list_topic.isEmpty())
            for(int i=0; i<list_topic.size(); ++i){
                auto result_subscription = ptr_mqtt_client->subscribe(list_topic.at(i));
                error_subscription(result_subscription->state());
            }
}

void mqtt_provider::error_changed(QMqttClient::ClientError error)
{
    qCritical("Mqtt client error code: '%d'", error);

    switch (error) {
    case QMqttClient::NoError:
        qCritical("No error occurred");
        break;
    case QMqttClient::InvalidProtocolVersion:
        qCritical("The broker does not accept a connection using the specified protocol version.");
        break;
    case QMqttClient::IdRejected:
        qCritical("The client ID is malformed. This might be related to its length.");
        break;
    case QMqttClient::ServerUnavailable:
        qCritical("The network connection has been established, but the service is unavailable on the broker side.");
        break;
    case QMqttClient::BadUsernameOrPassword:
        qCritical("The data in the username or password is malformed.");
        break;
    case QMqttClient::NotAuthorized:
        qCritical("The client is not authorized to connect.");
        break;
    case QMqttClient::TransportInvalid:
        qCritical("The underlying transport caused an error. For example, the connection might have been interrupted unexpectedly.");
        break;
    case QMqttClient::ProtocolViolation:
        qCritical("The client encountered a protocol violation, and therefore closed the connection.");
        break;
    case QMqttClient::UnknownError:
        qCritical("An unknown error occurred.");
        break;
    default:
        qCritical("Unknown error.");
        break;
    }
}

void mqtt_provider::error_subscription(const QMqttSubscription::SubscriptionState &error)
{
    qWarning("Mqtt client code: '%d'", error);

    switch (error) {
    case QMqttSubscription::Unsubscribed:
        qCritical("The topic has been unsubscribed from");
        break;
    case QMqttSubscription::SubscriptionPending:
        qCritical("A request for a subscription has been sent, but is has not been confirmed by the broker yet.");
        break;
    case QMqttSubscription::Subscribed:
        qCritical("The subscription was successful and messages will be received.");
        break;
    case QMqttSubscription::UnsubscriptionPending:
        qCritical("A request to unsubscribe from a topic has been sent, but it has not been confirmed by the broker yet.");
        break;
    case QMqttSubscription::Error:
        qCritical("An error occured.");
        break;
    default:
        qCritical("Unknown error.");
        break;
    }
}

