#ifndef CORE_SWEEP_CLIENT_H
#define CORE_SWEEP_CLIENT_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>
#include <QPointer>

#include "model/hackrf_info_model.h"
#include "model/message_log_model.h"
#include "model/params_spectr_model.h"
#include "model/ranges_template_model.h"

class user_interface;
class sweep_topic;
class SystemMonitorInterface;
class state_sweep_client;
class client_settings;
class sweep_message;
class sdr_info;
class data_log;
class system_monitor;
class ta_spectr;
class data_spectr;
class db_local_state;
class ranges_template;

class core_sweep_client : public QObject
{
    Q_OBJECT
public:
    explicit core_sweep_client(const QString &, const QString &, QObject *parent = nullptr);

    bool read_settings();
    void initialization(QQmlApplicationEngine *);
    void start_client();

public slots:
    void on_disconnect_from_host();

signals:
    void signal_state_connected();
    void signal_state_disconnected();

    void signal_sdr_info(const sdr_info &);
    void signal_data_log(const data_log &);
    void signal_system_monitor(const system_monitor &);
    void signal_data_spectr(const data_spectr &);
    void signal_open_db(const QString &);
    void signal_ranges_template(const ranges_template &);

private slots:
    void slot_publish_message(const QByteArray &);
    void slot_message_received(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());

private:
    QString m_file_settings;
    QString m_path_config;

    user_interface* ptr_user_interface {Q_NULLPTR};
    QMqttClient* ptr_mqtt_client {Q_NULLPTR};
    sweep_topic* ptr_sweep_topic {Q_NULLPTR};
    hackrf_info_model* ptr_hackrf_info_model {Q_NULLPTR};
    message_log_model* ptr_message_log_model {Q_NULLPTR};
    params_spectr_model* ptr_params_spectr_model {Q_NULLPTR};
    ranges_template_model* ptr_ranges_template_model {Q_NULLPTR};

    SystemMonitorInterface *ptr_system_monitor_interface {Q_NULLPTR};
    state_sweep_client *ptr_state_sweep_client {Q_NULLPTR};
    qint32 m_size_data_receive;

    // settings
    client_settings* ptr_client_settings {Q_NULLPTR};

    // ta spectr
    ta_spectr* ptr_ta_spectr_worker {Q_NULLPTR};
    QPointer<QThread> ptr_ta_spectr_thread;

    // database local
    db_local_state* ptr_db_local_state_worker {Q_NULLPTR};
    QPointer<QThread> ptr_db_local_thread;

    bool save_settings();
    bool read_template(const QString &);

    void update_log_state_change();
    void broker_disconnected();
    void ping_received();
    void connecting();

    void error_changed(QMqttClient::ClientError error);
    QStringList get_local_address_v4();
};

#endif // CORESWEEPCLIENT_H
