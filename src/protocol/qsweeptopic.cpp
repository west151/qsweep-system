#include "qsweeptopic.h"

QSweepTopic::QSweepTopic(QObject *parent) : QObject(parent)
{
}

QString QSweepTopic::sweepTopic(const Topic value) const
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
    case TOPIC_DATA:
        return topic_id + topic_data;
    case TOPIC_POWER_SPECTR:
        return topic_id + topic_spectr;
    case TOPIC_SYSTEM_MONITOR:
        return topic_id + topic_system_monitor;
    default:
        break;
    }

    return {};
}

QSweepTopic::Topic QSweepTopic::sweepTopic(const QString &value)
{
    if(value == topic_resource_id)
        return TOPIC_RESOURCE_ID;

    if(value == topic_id + topic_message_log)
        return TOPIC_MESSAGE_LOG;

    if(value == topic_id + topic_ctrl)
        return TOPIC_CTRL;

    if(value == topic_id + topic_info)
        return TOPIC_INFO;

    if(value == topic_id + topic_data)
        return TOPIC_DATA;

    if(value == topic_id + topic_spectr)
        return TOPIC_POWER_SPECTR;

    if(value == topic_id + topic_system_monitor)
        return TOPIC_SYSTEM_MONITOR;

    return TOPIC_UNKNOWN;
}

void QSweepTopic::setId(const QString &value)
{
    topic_id = value;
}

QString QSweepTopic::id() const
{
    return topic_id;
}
