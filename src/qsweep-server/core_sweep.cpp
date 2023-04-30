#include "core_sweep.h"
#include <QThread>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QNetworkInterface>

#include "hackrf_info.h"
#include "sweep_topic.h"
#include "spectrum_native_worker.h"
#include "spectrum_process_worker.h"
#include "parser_worker.h"
#include "sweep_message.h"
#include "params_spectr.h"

core_sweep::core_sweep(const QString &file, const QString &app_name, QObject *parent) : QObject(parent)
{
    qInfo("Init core ...");
    qInfo("Path config: '%s'", qUtf8Printable(file));

    QDir dir(file);
    if (!dir.exists())
        dir.mkpath(".");

    m_file_settings = file + QDir::separator() + app_name + ".conf";

    // create new file config
    QFileInfo file_config(m_file_settings);
    if(!file_config.exists())
        save_settings();


    ptr_sweep_topic = new sweep_topic(this);

    QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();
    QNetworkInterface eth;

    foreach(eth, allInterfaces) {
        QList<QNetworkAddressEntry> allEntries = eth.addressEntries();
        QNetworkAddressEntry entry;
        foreach (entry, allEntries) {
            QString ip_info;
            ip_info = entry.ip().toString() + "/" + entry.netmask().toString();

            qInfo("Host interfaces: '%s'", qUtf8Printable(ip_info));
        }
    }
}

bool core_sweep::read_settings()
{
    bool result(false);

    qInfo("Read file settings ...");

    if(!m_file_settings.isEmpty())
    {
        QFileInfo file_config(m_file_settings);

        if(file_config.exists())
        {
            QFile file(m_file_settings);

            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                ptr_server_settings = new server_settings(file.readAll());
                file.close();

                result = ptr_server_settings->is_valid();

                //ptr_sweep_topic->set_id(ptr_server_settings->id());

            }else{
                qCritical("Can't file open ('%s').", qUtf8Printable(m_file_settings));
                qCritical("Error: '%s'", qUtf8Printable(file.errorString()));
            }
        } else {
            qCritical("File '%s' does not exist!", qUtf8Printable(m_file_settings));

            fprintf(stderr, "Create new file settings: %s\n", qUtf8Printable(file_config.fileName()));

            save_settings();
        }
    }

    return result;
}

void core_sweep::initialization()
{
    m_run_sweep_worker = false;

    if(ptr_server_settings->spectrum_source_native())
        init_spectrum_native_worker();
    else
        init_spectrum_process_worker();

    // MQTT
    ptr_mqtt_client = new QMqttClient(this);

    // new style
    connect(ptr_mqtt_client, &QMqttClient::messageReceived,
            this, &core_sweep::slot_message_received);

    connect(ptr_mqtt_client, &QMqttClient::stateChanged,
            this, &core_sweep::update_log_state_change);
    connect(ptr_mqtt_client, &QMqttClient::disconnected,
            this, &core_sweep::broker_disconnected);
    connect(ptr_mqtt_client, &QMqttClient::connected,
            this, &core_sweep::ping_received);
    connect(ptr_mqtt_client, &QMqttClient::pingResponseReceived,
            this , &core_sweep::ping_received);
    connect(ptr_mqtt_client, &QMqttClient::errorChanged,
            this, &core_sweep::error_changed);

    // HackRF Info
    ptr_hackrf_info = new hackrf_info(this);
    // get hacrf info
    connect(this, &core_sweep::signal_run_hackrf_info,
            ptr_hackrf_info, &hackrf_info::slot_run_hackrf_info);
    // send result to broker
    connect(ptr_hackrf_info, &hackrf_info::signal_hackrf_info,
            this, &core_sweep::slot_publish_message);

    // System monitor
    ptrSystemMonitorWorker = new SystemMonitorWorker;
    ptrSystemMonitorThread = new QThread;
    ptrSystemMonitorWorker->moveToThread(ptrSystemMonitorThread);

    // start system monitor
    ptrTimer = new QTimer(this);
    connect(ptrTimer, &QTimer::timeout,
            ptrSystemMonitorWorker, &SystemMonitorWorker::runSystemMonitorWorker);
    // send result
    connect(ptrSystemMonitorWorker, &SystemMonitorWorker::signal_system_monitor_result,
            this, &core_sweep::slot_publish_message);

    ptrSystemMonitorThread->start();

    //    QTimer::singleShot(ptr_server_settings->delayed_launch(), this, &core_sweep::launching);
}

void core_sweep::start_server()
{
    // connect to broker
    if(ptr_server_settings && ptr_server_settings->is_valid())
    {
         qInfo("Start server ...");
         qInfo("Server settings: id '%s'", qUtf8Printable(ptr_server_settings->id()));
         qInfo("Server settings: host broker '%s'", qUtf8Printable(ptr_server_settings->host_broker()));
         qInfo("Server settings: port broker '%s'", qUtf8Printable(QString::number(ptr_server_settings->port_broker())));

         if(ptr_mqtt_client)
         {
             ptr_mqtt_client->setHostname(ptr_server_settings->host_broker());
             ptr_mqtt_client->setPort(ptr_server_settings->port_broker());

             if (ptr_mqtt_client->state() == QMqttClient::Disconnected)
                 ptr_mqtt_client->connectToHost();
         }

         // start system monitor
         ptrTimer->start(ptr_server_settings->system_monitor_interval());
    }
}

void core_sweep::slot_publish_message(const QByteArray &value)
{
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
    {
        const sweep_message send_data(value);

        if(send_data.is_valid())
        {
            if(send_data.type() == type_message::data_sdr_info)
                ptr_mqtt_client->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_info), value);

            if(send_data.type() == type_message::data_message_log)
                ptr_mqtt_client->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_message_log), value);

            if(send_data.type() == type_message::data_system_monitor)
                ptr_mqtt_client->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_system_monitor), value);

            if(send_data.type() == type_message::data_spectr)
                ptr_mqtt_client->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_power_spectr), value);
        }
    }
}

void core_sweep::slot_message_received(const QByteArray &message, const QMqttTopicName &topic)
{
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_ctrl)
    {
        const sweep_message ctrl_message(message);

        if(ctrl_message.is_valid())
        {
            // sdr info
            if(ctrl_message.type() == type_message::ctrl_info)
                emit signal_run_hackrf_info(message);

            // start/stop spectr
            if(ctrl_message.type() == type_message::ctrl_spectr)
            {
                const params_spectr params_spectr_data(ctrl_message.data_message());

                if(params_spectr_data.start_spectr())
                {
                    m_id_params_spectr = params_spectr_data.id_params();

                    if(!m_run_sweep_worker)
                        emit signal_run_spectr_worker(message);

                    if(m_run_sweep_worker){
                        emit signal_stop_spectr_worker();
                        emit signal_run_spectr_worker(message);
                    }
                }else{
                    m_id_params_spectr.clear();
                    emit signal_stop_spectr_worker();
                }
            }
        }
    }
}

void core_sweep::slot_sweep_worker(const bool &on)
{
    m_run_sweep_worker = on;
}

void core_sweep::init_spectrum_native_worker()
{
    qInfo("Init spectrum native.");

    ptr_spectrum_native_worker = new spectrum_native_worker;
    ptr_spectrum_native_thread = new QThread;
    ptr_spectrum_native_worker->moveToThread(ptr_spectrum_native_thread);

    connect(this, &core_sweep::signal_run_spectr_worker,
            ptr_spectrum_native_worker, &spectrum_native_worker::slot_run_sweep_worker);

    connect(ptr_spectrum_native_worker, &spectrum_native_worker::signal_sweep_message,
            this, &core_sweep::slot_publish_message);

    connect(ptr_spectrum_native_worker, &spectrum_native_worker::signal_sweep_worker,
            this, &core_sweep::slot_sweep_worker);

    // Power spectr
    connect(spectrum_native_worker::getInstance(), &spectrum_native_worker::signal_sweep_message,
            this, &core_sweep::slot_publish_message);
    // stop spectr
    connect(this, &core_sweep::signal_stop_spectr_worker,
            spectrum_native_worker::getInstance(), &spectrum_native_worker::slot_stop_sweep_worker);

    ptr_spectrum_native_thread->start();
}

void core_sweep::init_spectrum_process_worker()
{
    qInfo("Init spectrum process.");

    // run process hackrf_sweep
    ptr_spectrum_process_worker = new spectrum_process_worker(ptr_server_settings->spectrum_process_name());
    ptr_spectrum_process_thread = new QThread;
    ptr_spectrum_process_worker->moveToThread(ptr_spectrum_process_thread);

    // stop spectr
    connect(this, &core_sweep::signal_stop_spectr_worker,
            ptr_spectrum_process_worker, &spectrum_process_worker::slot_stop_process_worker);

    // send message to published
    connect(ptr_spectrum_process_worker, &spectrum_process_worker::signal_message,
            this, &core_sweep::slot_publish_message);

    ptr_spectrum_process_thread->start();

    // parser hackrf_sweep stdout
    ptr_parser_worker = new parser_worker;
    ptr_parser_thread = new QThread;
    ptr_parser_worker->moveToThread(ptr_parser_thread);

    // run(get config) spectr
    connect(this, &core_sweep::signal_run_spectr_worker,
            ptr_parser_worker, &parser_worker::slot_run_parser_worker);

    // run spectr
    connect(ptr_parser_worker, &parser_worker::signal_run_process_worker,
            ptr_spectrum_process_worker, &spectrum_process_worker::slot_run_process_worker);

    connect(ptr_spectrum_process_worker, &spectrum_process_worker::signal_output_line,
            ptr_parser_worker, &parser_worker::slot_input_line);    

    // Power spectr
    connect(ptr_parser_worker, &parser_worker::signal_data_spectr_message,
            this, &core_sweep::slot_publish_message);

    // stop parser
    connect(this, &core_sweep::signal_stop_spectr_worker,
            ptr_parser_worker, &parser_worker::slot_stop_parser_worker);

    ptr_parser_thread->start();
}

bool core_sweep::save_settings()
{
    bool isSave(false);

    if(!m_file_settings.isEmpty())
    {
        QFileInfo file_config(m_file_settings);
        bool fileExists = file_config.exists();

        QFile file(m_file_settings);

        if(file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            if(fileExists)
            {
                if(ptr_server_settings)
                    file.write(ptr_server_settings->to_json());
            } else {
                auto defaultSettings = server_settings();
                QStringList local_address = get_local_address_v4();
                if(!local_address.isEmpty())
                    defaultSettings.set_host_broker(local_address.at(0));
                // set default ip
                file.write(defaultSettings.to_json());
            }

            file.close();
            isSave = true;
        }else{
            qCritical("Can't file open ('%s').", qUtf8Printable(m_file_settings));
            qCritical("Error: '%s'", qUtf8Printable(file.errorString()));
        }
    }

    return isSave;
}

void core_sweep::update_log_state_change()
{
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
    {
        auto subscription = ptr_mqtt_client->subscribe(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_ctrl), 0);

        if (!subscription)
        {
#ifdef QT_DEBUG
            qDebug() << "Could not subscribe. Is there a valid connection?";
#endif
            return;
        }
    }

#ifdef QT_DEBUG
    qDebug() << QDateTime::currentDateTime().toString()
             << QLatin1String(": State Change")
             << ptr_mqtt_client->state();
#endif
}

void core_sweep::broker_disconnected()
{
    qInfo("broker_disconnected");
}

void core_sweep::ping_received()
{
    qInfo("ping_received");
}

void core_sweep::connecting()
{
    qInfo("connecting");
}

QStringList core_sweep::get_local_address_v4()
{
    QStringList local_address;

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            local_address.append(address.toString());
            qInfo("IP address: '%s'", qUtf8Printable(address.toString()));
        }
    }

    return local_address;
}

void core_sweep::error_changed(QMqttClient::ClientError error)
{
    qCritical("Mqtt client error code: '%d'", error);

    switch (error) {
    case QMqttClient::NoError:
        qCritical("No error occurred");
        break;
    case QMqttClient::InvalidProtocolVersion:
        qCritical("The broker does not accept a connection using the specified protocol version.");
        break;
    case QMqttClient::IdRejected:
        qCritical("The client ID is malformed. This might be related to its length.");
        break;
    case QMqttClient::ServerUnavailable:
        qCritical("The network connection has been established, but the service is unavailable on the broker side.");
        break;
    case QMqttClient::BadUsernameOrPassword:
        qCritical("The data in the username or password is malformed.");
        break;
    case QMqttClient::NotAuthorized:
        qCritical("The client is not authorized to connect.");
        break;
    case QMqttClient::TransportInvalid:
        qCritical("The underlying transport caused an error. For example, the connection might have been interrupted unexpectedly.");
        break;
    case QMqttClient::ProtocolViolation:
        qCritical("The client encountered a protocol violation, and therefore closed the connection.");
        break;
    case QMqttClient::UnknownError:
        qCritical("An unknown error occurred.");
        break;
    default:
        qCritical("Unknown error.");
        break;
    }
}
