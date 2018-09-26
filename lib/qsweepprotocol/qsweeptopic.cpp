#include "qsweeptopic.h"

static const QString topic_message_log = QLatin1String("sdr/message/log");
static const QString topic_ctrl = QLatin1String("sdr/ctrl");
static const QString topic_info = QLatin1String("sdr/info");
static const QString topic_data = QLatin1String("sdr/data");
static const QString topic_spectr = QLatin1String("sdr/spectr");
static const QString topic_system_monitor = QLatin1String("sdr/system/monitor");

QSweepTopic::QSweepTopic(QObject *parent) : QObject(parent)
{

}

QString QSweepTopic::sweepTopic(const Topic value) const
{
    switch (value) {
    case TOPIC_MESSAGE_LOG:
        return topic_message_log;
    case TOPIC_CTRL:
        return topic_ctrl;
    case TOPIC_INFO:
        return topic_info;
    case TOPIC_DATA:
        return topic_data;
    case TOPIC_POWER_SPECTR:
        return topic_spectr;
    case TOPIC_SYSTEM_MONITOR:
        return  topic_system_monitor;
    default:
        break;
    }

    return {};
}

QSweepTopic::Topic QSweepTopic::sweepTopic(const QString &value)
{
    if(value == topic_message_log)
        return TOPIC_MESSAGE_LOG;

    if(value == topic_ctrl)
        return TOPIC_CTRL;

    if(value == topic_info)
        return TOPIC_INFO;

    if(value == topic_data)
        return TOPIC_DATA;

    if(value == topic_spectr)
        return TOPIC_POWER_SPECTR;

    if(value == topic_system_monitor)
        return TOPIC_SYSTEM_MONITOR;

    return TOPIC_UNKNOWN;
}
