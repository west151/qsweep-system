#ifndef SYSTEMMONITORWORKER_H
#define SYSTEMMONITORWORKER_H

#include <QObject>

class SystemMonitorWorker : public QObject
{
    Q_OBJECT
public:
    explicit SystemMonitorWorker(QObject *parent = nullptr);

signals:
    void signal_system_monitor_result(const QByteArray &value);

public slots:
    void runSystemMonitorWorker();
};

#endif // SYSTEMMONITORWORKER_H
