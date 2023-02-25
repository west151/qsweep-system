#ifndef SYSTEM_MONITOR_WORKER_H
#define SYSTEM_MONITOR_WORKER_H

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

#endif // SYSTEM_MONITOR_WORKER_H
