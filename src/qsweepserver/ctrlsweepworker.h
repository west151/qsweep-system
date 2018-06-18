#ifndef CTRLSWEEPWORKER_H
#define CTRLSWEEPWORKER_H

#include <QObject>
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

#include "qsweepparams.h"

class CtrlSweepWorker : public QObject
{
    Q_OBJECT
public:
    explicit CtrlSweepWorker(QObject *parent = nullptr);

//public slots:
//    void startSweepWorkerTest();
    void startSweepWorker(const QSweepParams &);

signals:
    //void sendRunSweepWorker();
    void sendStopSweepWorker();
    void sendRunSweepWorker(const QSweepParams &);

private:
    QMqttClient *m_client;
};

#endif // CTRLSWEEPWORKER_H
