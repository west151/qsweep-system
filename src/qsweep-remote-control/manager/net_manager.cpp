#include "net_manager.h"

#include "mqtt_provider_settings.h"
#include "mqtt_provider.h"
#include "sweep_message.h"
#include "sweep_topic.h"

net_manager::net_manager(QObject *parent) : QObject(parent),
    ptr_sweep_topic(new sweep_topic(this)),
    ptr_mqtt_provider(new mqtt_provider(this))
{

}

void net_manager::set_settings(const QByteArray &value)
{
    ptr_mqtt_provider_settings = new mqtt_provider_settings(value);

    ptr_mqtt_provider->set_host_broker(ptr_mqtt_provider_settings->host_broker());
    ptr_mqtt_provider->set_port_broker(ptr_mqtt_provider_settings->port_broker());
}

void net_manager::slot_start()
{
    ptr_mqtt_provider->launching();
}

void net_manager::slot_publish_ctrl(const QByteArray &value)
{
    if((ptr_mqtt_provider->is_ready())&&(value.size()>0))
    {
        const sweep_message send_data(value);

        if(send_data.is_valid())
        {
            switch (send_data.type()) {
            case type_message::ctrl_spectr:
                ptr_mqtt_provider->slot_publish_message(send_data.to_json(),
                                                        ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_ctrl));
                break;
            case type_message::ctrl_db:
                ptr_mqtt_provider->slot_publish_message(send_data.to_json(),
                                                        ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_db_ctrl));
                break;
            default:
                break;
            }
        }
    }

////#ifdef QT_DEBUG
////            qDebug() << Q_FUNC_INFO << tr("Data sending to host result:") << send_data.to_json();
////#endif
}

