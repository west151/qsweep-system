#include "sweep_topic.h"

sweep_topic::sweep_topic(QObject *parent) : QObject(parent)
{
}

QString sweep_topic::sweep_topic_by_type(const topic value) const
{
    switch (value) {
    case topic_resource_id:
        return str_topic_resource_id;
    case topic_message_log:
        return str_topic_id + str_topic_message_log;
    case topic_ctrl:
        return str_topic_id + str_topic_ctrl;
    case topic_info:
        return str_topic_id + str_topic_info;
    case topic_power_spectr:
        return str_topic_id + str_topic_spectr;
    case topic_system_monitor:
        return str_topic_id + str_topic_system_monitor;
    case topic_db_ctrl:
        return str_topic_id + str_topic_db_ctrl;
    default:
        break;
    }

    return {};
}

sweep_topic::topic sweep_topic::sweep_topic_by_str(const QString &value)
{
    if(value == str_topic_resource_id)
        return topic_resource_id;

    if(value == str_topic_id + str_topic_message_log)
        return topic_message_log;

    if(value == str_topic_id + str_topic_ctrl)
        return topic_ctrl;

    if(value == str_topic_id + str_topic_info)
        return topic_info;

    if(value == str_topic_id + str_topic_spectr)
        return topic_power_spectr;

    if(value == str_topic_id + str_topic_system_monitor)
        return topic_system_monitor;

    if(value == str_topic_id + str_topic_db_ctrl)
        return topic_db_ctrl;

    return topic_unknown;
}

void sweep_topic::set_id(const QString &value)
{
    str_topic_id = value;
}

QString sweep_topic::id() const
{
    return str_topic_id;
}
