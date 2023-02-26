#ifndef CORE_SWEEP_H
#define CORE_SWEEP_H

// mosquitto configure
// /etc/mosquitto/mosquitto.conf
//
// port 1883
// allow_anonymous true
// bind_address 0.0.0.0

#include <QObject>
#include <QPointer>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

#include "settings/server_settings.h"
#include "system_monitor_worker.h"

class QTimer;
class hackrf_info;
class spectrum_native_worker;
class sweep_topic;
class spectrum_process_worker;
class parser_worker;

class core_sweep : public QObject
{
    Q_OBJECT
public:
    explicit core_sweep(const QString &, const QString &, QObject *parent = nullptr);
    bool read_settings();
    void initialization();
    void start_server();

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
    QString m_file_settings;

    hackrf_info* ptr_hackrf_info {Q_NULLPTR};

    spectrum_native_worker* ptr_spectrum_native_worker {Q_NULLPTR};
    QPointer<QThread> ptr_spectrum_native_thread;
    void init_spectrum_native_worker();

    QMqttClient* ptr_mqtt_client {Q_NULLPTR};
    sweep_topic* ptr_sweep_topic {Q_NULLPTR};
    server_settings* ptr_server_settings {Q_NULLPTR};

    QTimer* ptrTimer {Q_NULLPTR};
    SystemMonitorWorker* ptrSystemMonitorWorker {Q_NULLPTR};
    QPointer<QThread> ptrSystemMonitorThread;

    // run process hackrf_sweep
    spectrum_process_worker* ptr_spectrum_process_worker {Q_NULLPTR};
    QPointer<QThread> ptr_spectrum_process_thread;
    void init_spectrum_process_worker();

    // parser hackrf_sweep stdout
    parser_worker* ptr_parser_worker {Q_NULLPTR};
    QPointer<QThread> ptr_parser_thread;

    bool save_settings();

    void update_log_state_change();
    void broker_disconnected();
    void ping_received();
    void connecting();
    QStringList get_local_address_v4();

    void error_changed(QMqttClient::ClientError error);
};

#endif // CORE_SWEEP_H
