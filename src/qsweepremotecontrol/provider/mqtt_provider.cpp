#include "mqtt_provider.h"

#include <QDateTime>

mqtt_provider::mqtt_provider(QObject *parent) : QObject(parent),
    ptr_mqtt_client(new QMqttClient(this))
{
    m_state_init_hash.insert(param_client_id, false);
    m_state_init_hash.insert(param_host_broker, false);
    m_state_init_hash.insert(param_port_broker, false);
    m_state_init_hash.insert(provider_is_ready, false);

    connect(ptr_mqtt_client, &QMqttClient::messageReceived,
            this, &mqtt_provider::slot_received_message);

    connect(ptr_mqtt_client, &QMqttClient::stateChanged,
            this, &mqtt_provider::slot_update_state_change);

    connect(ptr_mqtt_client, &QMqttClient::errorChanged,
            this, &mqtt_provider::slot_error_changed);
}

void mqtt_provider::set_host_broker(const QString &value)
{
    m_host_broker = value;

    if(!m_host_broker.isEmpty())
        m_state_init_hash.insert(param_host_broker, true);
}

QString mqtt_provider::host_broker() const
{
    return m_host_broker;
}

void mqtt_provider::set_port_broker(const quint16 &value)
{
    m_port_broker = value;

    if(m_port_broker > 0)
        m_state_init_hash.insert(param_port_broker, true);
}

quint16 mqtt_provider::port_broker() const
{
    return m_port_broker;
}

void mqtt_provider::set_client_id(const QString &value)
{
    m_client_id = value;

    if(!m_client_id.isEmpty())
        m_state_init_hash.insert(param_client_id, true);
}

QString mqtt_provider::client_id() const
{
    return m_client_id;
}

void mqtt_provider::launching()
{
    // connect to mqtt broker
    if(ptr_mqtt_client)
    {
        ptr_mqtt_client->setHostname(m_host_broker);
        ptr_mqtt_client->setPort(m_port_broker);
        ptr_mqtt_client->setClientId(m_client_id);

        if (ptr_mqtt_client->state() == QMqttClient::Disconnected)
            ptr_mqtt_client->connectToHost();
    }
}

void mqtt_provider::stopping()
{
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
        ptr_mqtt_client->disconnectFromHost();
}

bool mqtt_provider::is_ready()
{
    return m_state_init_hash.value(provider_is_ready, false);
}

bool mqtt_provider::is_init_state()
{
    QList<init_state> list_init_state;

    list_init_state << param_client_id
                    << param_host_broker
                    << param_port_broker;

    for(const auto &state : list_init_state)
        if(m_state_init_hash.value(state, false) == false)
            return false;

    return true;
}

void mqtt_provider::slot_publish_message(const QByteArray &pdata, const QString &address)
{
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
        ptr_mqtt_client->publish(address, pdata);
}

void mqtt_provider::slot_unsubscribe_broker(const QStringList &list_topic)
{
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
        if(!list_topic.isEmpty())
            for(int i=0; i<list_topic.size(); ++i){
                ptr_mqtt_client->unsubscribe(list_topic.at(i));
                m_list_of_subscribe.removeAll(list_topic.at(i));
            }
}

void mqtt_provider::slot_subscribe_broker(const QStringList &list_topic)
{
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
        if(!list_topic.isEmpty())
            for(int i=0; i<list_topic.size(); ++i){
                auto result_subscription = ptr_mqtt_client->subscribe(list_topic.at(i));
                error_subscription(result_subscription->state());

                if(result_subscription->state() == QMqttSubscription::Subscribed)
                    m_list_of_subscribe.append(list_topic.at(i));
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

void mqtt_provider::slot_error_changed(QMqttClient::ClientError error)
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

void mqtt_provider::slot_received_message(const QByteArray &rdata, const QMqttTopicName &topic)
{
    if(topic.isValid()){
        const QString address(topic.name());
        emit signal_received_message(rdata, address);
    }
}

void mqtt_provider::slot_update_state_change()
{
#ifdef QT_DEBUG
    const QString content = QDateTime::currentDateTime().toString()
            + QLatin1String(": State Change ")
            + QString::number(ptr_mqtt_client->state());
    qDebug().noquote() << content;
#endif

    // Subscribers topic
    if (ptr_mqtt_client->state() == QMqttClient::Connected){
        emit signal_state_connected();
        m_state_init_hash.insert(provider_is_ready, true);
    }

    if (ptr_mqtt_client->state() == QMqttClient::Disconnected){
        emit signal_state_disconnected();
        m_state_init_hash.insert(provider_is_ready, false);
    }
}

void mqtt_provider::slot_state_connected()
{
    if(m_list_of_subscribe.size() > 0)
    {
        for (int i=0; i<m_list_of_subscribe.size(); ++i)
        {
            auto subscription = ptr_mqtt_client->subscribe(m_list_of_subscribe.at(i));

            if (!subscription)
                qCritical("Could not subscribe. Is there a valid connection?");
        }
    }
}

//#include "mqtt_provider.h"
//#include "sweep_write_settings.h"
//#include "sweep_message.h"
//#include "broker_ctrl.h"

//#include <QDateTime>

//mqtt_provider::mqtt_provider(QObject *parent) : QObject(parent)
//{
//    m_subscribe_data.append(sweep_topic::topic_ctrl);
//    m_subscribe_ctrl.append(sweep_topic::topic_db_ctrl);
//}

//void mqtt_provider::initialization()
//{
//    ptr_mqtt_client = new QMqttClient(this);

//    connect(ptr_mqtt_client, &QMqttClient::messageReceived,
//            this, &mqtt_provider::slot_message_received);
//    connect(ptr_mqtt_client, &QMqttClient::stateChanged,
//            this, &mqtt_provider::update_state_change);
//    connect(ptr_mqtt_client, &QMqttClient::disconnected,
//            this, &mqtt_provider::broker_disconnected);
//    connect(ptr_mqtt_client, &QMqttClient::connected,
//            this, &mqtt_provider::ping_received);
//    connect(ptr_mqtt_client, &QMqttClient::pingResponseReceived,
//            this , &mqtt_provider::ping_received);
//    connect(ptr_mqtt_client, &QMqttClient::errorChanged,
//            this, &mqtt_provider::error_changed);

//    ptr_sweep_topic = new sweep_topic(this);

//    connect(this, &mqtt_provider::signal_state_connected,
//            this, &mqtt_provider::slot_state_connected);
//}

//void mqtt_provider::slot_message_received(const QByteArray &message, const QMqttTopicName &topic)
//{
//    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_db_ctrl)
//    {
//        const sweep_message data_received(message, false);

//        if(data_received.is_valid())
//        {
//            if(data_received.type() == type_message::ctrl_db)
//            {
//                const broker_ctrl broker_ctrl_data(data_received.data_message(), false);

//                if(broker_ctrl_data.ctrl_type()==broker_ctrl_type::subscribe)
//                    subscribe_broker(broker_ctrl_data.topic_list());

//                if(broker_ctrl_data.ctrl_type()==broker_ctrl_type::unsubscribe)
//                    unsubscribe_broker(broker_ctrl_data.topic_list());

//#ifdef QT_DEBUG
//                qDebug().noquote() << "broker_ctrl"
//                                   << broker_ctrl_data.ctrl_type()
//                                   << broker_ctrl_data.topic_list();
//#endif
//            }
//        }
//    }

//    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_power_spectr)
//    {
//        const sweep_message data_received(message, false);

//        if(data_received.is_valid())
//            if(data_received.type() == type_message::data_spectr)
//                emit signal_received_data(message);
//    }

//    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_ctrl)
//    {
//        const sweep_message data_received(message, false);

//        if(data_received.is_valid())
//            if(data_received.type() == type_message::ctrl_spectr)
//                emit signal_received_data(message);
//    }
//}

//void mqtt_provider::slot_state_connected()
//{
//    if(m_subscribe_ctrl.size()>0)
//    {
//        for (int i=0; i<m_subscribe_ctrl.size(); ++i)
//        {
//            auto subscription = ptr_mqtt_client->subscribe(ptr_sweep_topic->sweep_topic_by_type(m_subscribe_ctrl.at(i)));
//            if (!subscription)
//            {
//#ifdef QT_DEBUG
//                qDebug() << "Could not subscribe. Is there a valid connection?";
//#endif
//            }else{
//#ifdef QT_DEBUG
//                qDebug() << "subscribe:" << ptr_sweep_topic->sweep_topic_by_type(m_subscribe_ctrl.at(i));
//#endif
//            }
//        }
//    }

//    if(m_subscribe_data.size()>0)
//    {
//        for (int i=0; i<m_subscribe_data.size(); ++i)
//        {
//            auto subscription = ptr_mqtt_client->subscribe(ptr_sweep_topic->sweep_topic_by_type(m_subscribe_data.at(i)));
//            if (!subscription)
//            {
//#ifdef QT_DEBUG
//                qDebug() << "Could not subscribe. Is there a valid connection?";
//#endif
//            }else{
//#ifdef QT_DEBUG
//                qDebug() << "subscribe:" << ptr_sweep_topic->sweep_topic_by_type(m_subscribe_data.at(i));
//#endif
//            }
//        }
//    }
//}
