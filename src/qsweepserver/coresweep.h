#ifndef CORESWEEP_H
#define CORESWEEP_H

#include <QObject>
#include <QPointer>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

#include "settings/sweepserversettings.h"
#include "systemmonitorworker.h"

class HackrfInfo;
class SweepWorker;
class sweep_topic;
class QTimer;

class CoreSweep : public QObject
{
    Q_OBJECT
public:
    explicit CoreSweep(const QString &, QObject *parent = nullptr);

signals:
    void signal_run_hackrf_info(const QByteArray &);
    void signal_run_spectr_worker(const QByteArray &);
    void signal_stop_spectr_worker();

private slots:
    void slot_publish_message(const QByteArray &);
    void slot_message_received(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());
    void slot_sweep_worker(const bool &);

private:
    bool m_run_sweep_worker;
    QString m_id_params_spectr;

    HackrfInfo* ptrHackrfInfo {Q_NULLPTR};
    SweepWorker* ptrSweepWorker {Q_NULLPTR};
    QPointer<QThread> ptrSweepThread;

    QMqttClient* ptrMqttClient {Q_NULLPTR};
    sweep_topic* ptrSweepTopic {Q_NULLPTR};
    SweepServerSettings* ptrSweepServerSettings {Q_NULLPTR};

    QTimer* ptrTimer {Q_NULLPTR};
    SystemMonitorWorker* ptrSystemMonitorWorker {Q_NULLPTR};
    QPointer<QThread> ptrSystemMonitorThread;

    bool readSettings(const QString &);
    bool saveSettings(const QString &);
    void initialization();
    void launching();

    void updateLogStateChange();
    void brokerDisconnected();
    void pingReceived();
    void connecting();

    void errorChanged(QMqttClient::ClientError error);
};

#endif // CORESWEEP_H
