#ifndef CORE_SWEEP_H
#define CORE_SWEEP_H

#include <QObject>
#include <QPointer>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

#include "settings/server_settings.h"
#include "systemmonitorworker.h"

class QTimer;
class hackrf_info;
class spectrum_native_worker;
class sweep_topic;
class process_worker;
class parser_worker;

class core_sweep : public QObject
{
    Q_OBJECT
public:
    explicit core_sweep(const QString &, QObject *parent = nullptr);

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

    hackrf_info* ptr_hackrf_info {Q_NULLPTR};

    spectrum_native_worker* ptr_spectrum_native_worker {Q_NULLPTR};
    QPointer<QThread> ptr_spectrum_native_thread;
    void init_spectrum_native_worker();

    QMqttClient* ptrMqttClient {Q_NULLPTR};
    sweep_topic* ptr_sweep_topic {Q_NULLPTR};
    server_settings* ptr_server_settings {Q_NULLPTR};

    QTimer* ptrTimer {Q_NULLPTR};
    SystemMonitorWorker* ptrSystemMonitorWorker {Q_NULLPTR};
    QPointer<QThread> ptrSystemMonitorThread;

    // run process hackrf_sweep
    process_worker* ptr_process_worker {Q_NULLPTR};
    QPointer<QThread> ptr_process_thread;
    void init_process_worker();

    // parser hackrf_sweep stdout
    parser_worker* ptr_parser_worker {Q_NULLPTR};
    QPointer<QThread> ptr_parser_thread;

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

#endif // CORE_SWEEP_H
