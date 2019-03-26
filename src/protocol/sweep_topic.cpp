#include "sweep_topic.h"

sweep_topic::sweep_topic(QObject *parent) : QObject(parent)
{
}

QString sweep_topic::sweep_topic_by_type(const topic value) const
{
    switch (value) {
    case TOPIC_RESOURCE_ID:
        return topic_resource_id;
    case TOPIC_MESSAGE_LOG:
        return topic_id + topic_message_log;
    case TOPIC_CTRL:
        return topic_id + topic_ctrl;
    case TOPIC_INFO:
        return topic_id + topic_info;
    case TOPIC_POWER_SPECTR:
        return topic_id + topic_spectr;
    case TOPIC_SYSTEM_MONITOR:
        return topic_id + topic_system_monitor;
    case TOPIC_DB_CTRL:
        return topic_id + topic_db_ctrl;
    default:
        break;
    }

    return {};
}

sweep_topic::topic sweep_topic::sweep_topic_by_str(const QString &value)
{
    if(value == topic_resource_id)
        return TOPIC_RESOURCE_ID;

    if(value == topic_id + topic_message_log)
        return TOPIC_MESSAGE_LOG;

    if(value == topic_id + topic_ctrl)
        return TOPIC_CTRL;

    if(value == topic_id + topic_info)
        return TOPIC_INFO;

    if(value == topic_id + topic_spectr)
        return TOPIC_POWER_SPECTR;

    if(value == topic_id + topic_system_monitor)
        return TOPIC_SYSTEM_MONITOR;

    if(value == topic_id + topic_db_ctrl)
        return TOPIC_DB_CTRL;

    return TOPIC_UNKNOWN;
}

void sweep_topic::set_id(const QString &value)
{
    topic_id = value;
}

QString sweep_topic::id() const
{
    return topic_id;
}
