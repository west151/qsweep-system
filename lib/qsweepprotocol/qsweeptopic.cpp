#include "qsweeptopic.h"

static const QString topic_message_log = QLatin1String("sdr/message/log");
static const QString topic_ctrl = QLatin1String("sdr/ctrl");
static const QString topic_info = QLatin1String("sdr/info");
static const QString topic_data = QLatin1String("sdr/data");
static const QString topic_spectr = QLatin1String("sdr/spectr");

QSweepTopic::QSweepTopic(QObject *parent) : QObject(parent)
{

}

QString QSweepTopic::sweepTopic(const Topic value) const
{
    switch (value) {
    case TOPIC_MESSAGE_LOG:
        return topic_message_log;
        break;
    case TOPIC_CTRL:
        return topic_ctrl;
        break;
    case TOPIC_INFO:
        return topic_info;
        break;
    case TOPIC_DATA:
        return topic_data;
        break;
    case TOPIC_SPECTR:
        return topic_spectr;
        break;
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
        return TOPIC_SPECTR;

    return TOPIC_UNKNOWN;
}
