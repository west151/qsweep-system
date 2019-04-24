#ifndef QSWEEPTOPIC_H
#define QSWEEPTOPIC_H

#include <QObject>

class sweep_topic : public QObject
{
    Q_OBJECT
public:
    enum topic : qint32 {
        topic_unknown,
        topic_resource_id,
        topic_message_log,
        topic_ctrl,
        topic_db_ctrl,
        topic_info,
        topic_power_spectr,
        topic_system_monitor
    };

    explicit sweep_topic(QObject *parent = nullptr);

    QString sweep_topic_by_type(const topic value = topic_unknown) const;
    topic sweep_topic_by_str(const QString &value = "");

    void set_id(const QString &);
    QString id()const;

private:
    QString str_topic_id = QLatin1String("sdr");
    QString str_topic_resource_id = QLatin1String("resource/id");
    // ctrl
    QString str_topic_ctrl = QLatin1String("/ctrl");
    QString str_topic_db_ctrl = QLatin1String("/db/ctrl");
    // data result
    QString str_topic_message_log = QLatin1String("/message/log");
    QString str_topic_info = QLatin1String("/info");
    QString str_topic_spectr = QLatin1String("/spectr");
    QString str_topic_system_monitor = QLatin1String("/system/monitor");
};

#endif // QSWEEPTOPIC_H
