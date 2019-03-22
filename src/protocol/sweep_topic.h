#ifndef QSWEEPTOPIC_H
#define QSWEEPTOPIC_H

#include <QObject>

class sweep_topic : public QObject
{
    Q_OBJECT
public:
    enum Topic : qint32 {
        TOPIC_UNKNOWN,
        TOPIC_RESOURCE_ID,
        TOPIC_MESSAGE_LOG,
        TOPIC_CTRL,
        TOPIC_INFO,
        TOPIC_DATA,
        TOPIC_POWER_SPECTR,
        TOPIC_SYSTEM_MONITOR
    };

    explicit sweep_topic(QObject *parent = nullptr);

    QString sweep_topic_by_type(const Topic value = TOPIC_UNKNOWN) const;
    Topic sweep_topic_by_str(const QString &value = "");

    void set_id(const QString &);
    QString id()const;

private:
    QString topic_id = QLatin1String("sdr");
    QString topic_resource_id = QLatin1String("resource/id");
    QString topic_message_log = QLatin1String("/message/log");
    QString topic_ctrl = QLatin1String("/ctrl");
    QString topic_info = QLatin1String("/info");
    QString topic_data = QLatin1String("/data");
    QString topic_spectr = QLatin1String("/spectr");
    QString topic_system_monitor = QLatin1String("/system/monitor");
};

#endif // QSWEEPTOPIC_H
