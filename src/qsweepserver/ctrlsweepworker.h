#ifndef CTRLSWEEPWORKER_H
#define CTRLSWEEPWORKER_H

#include <QObject>
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

class SweepParams;

class CtrlSweepWorker : public QObject
{
    Q_OBJECT
public:
    explicit CtrlSweepWorker(QObject *parent = nullptr);

public slots:
    void startSweepWorkerTest();

signals:
    void sendRunSweepWorker();
    void sendStopSweepWorker();
    void sendParamsSweepWorker(const SweepParams &);

private:
    QMqttClient *m_client;
};

#endif // CTRLSWEEPWORKER_H
