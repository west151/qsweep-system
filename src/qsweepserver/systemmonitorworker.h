#ifndef SYSTEMMONITORWORKER_H
#define SYSTEMMONITORWORKER_H

#include <QObject>

#include "qsweepsystemmonitor.h"

class SystemMonitorWorker : public QObject
{
    Q_OBJECT
public:
    explicit SystemMonitorWorker(QObject *parent = nullptr);

signals:
    void sendSystemMonitorResult(const QByteArray &value);

public slots:
    void runSystemMonitorWorker();
};

#endif // SYSTEMMONITORWORKER_H
