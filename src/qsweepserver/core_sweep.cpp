#include "core_sweep.h"
#include <QThread>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "hackrf_info.h"
#include "spectrum_native_worker.h"
#include "sweep_topic.h"
#include "spectrum_process_worker.h"
#include "parser_worker.h"
#include "sweep_message.h"
#include "params_spectr.h"

static const QString config_suffix(QString(".conf"));

core_sweep::core_sweep(const QString &file, QObject *parent) : QObject(parent)
{
    // read file settings
    const auto isSettings = readSettings(file);

    if(isSettings)
    {
        initialization();
    } else {
        qCritical("Error: Read settings.");
    }
}

void core_sweep::slot_publish_message(const QByteArray &value)
{
    if (ptrMqttClient->state() == QMqttClient::Connected)
    {
        const sweep_message send_data(value);

        if(send_data.is_valid())
        {
            if(send_data.type() == type_message::data_sdr_info)
                ptrMqttClient->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_info), value);

            if(send_data.type() == type_message::data_message_log)
                ptrMqttClient->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_message_log), value);

            if(send_data.type() == type_message::data_system_monitor)
                ptrMqttClient->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_system_monitor), value);

            if(send_data.type() == type_message::data_spectr)
                ptrMqttClient->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_power_spectr), value);
        }
    }
}

void core_sweep::slot_message_received(const QByteArray &message, const QMqttTopicName &topic)
{
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_ctrl)
    {
        const sweep_message ctrl_message(message, false);

        if(ctrl_message.is_valid())
        {
            // sdr info
            if(ctrl_message.type() == type_message::ctrl_info)
                emit signal_run_hackrf_info(message);

            // start/stop spectr
            if(ctrl_message.type() == type_message::ctrl_spectr)
            {
                const params_spectr params_spectr_data(ctrl_message.data_message(), false);

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
#ifdef QT_DEBUG
    qDebug() << "Init spectrum native";
#endif

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
#ifdef QT_DEBUG
    qDebug() << "Init spectrum process";
#endif

    // run process hackrf_sweep
    ptr_spectrum_process_worker = new spectrum_process_worker;
    ptr_spectrum_process_thread = new QThread;
    ptr_spectrum_process_worker->moveToThread(ptr_spectrum_process_thread);

    connect(this, &core_sweep::signal_run_spectr_worker,
            ptr_spectrum_process_worker, &spectrum_process_worker::slot_run_process_worker);

    ptr_spectrum_process_thread->start();

    // parser hackrf_sweep stdout
    ptr_parser_worker = new parser_worker;
    ptr_parser_thread = new QThread;
    ptr_parser_worker->moveToThread(ptr_parser_thread);

    connect(ptr_spectrum_process_worker, &spectrum_process_worker::signal_output_line,
            ptr_parser_worker, &parser_worker::slot_input_line);

    // Power spectr
    connect(ptr_parser_worker, &parser_worker::signal_data_spectr_message,
            this, &core_sweep::slot_publish_message);

    ptr_parser_thread->start();
}

bool core_sweep::readSettings(const QString &file)
{
    bool isRead(false);

    if(!file.isEmpty())
    {
        QFileInfo info(file);
        QString fileConfig(info.absolutePath()+QDir::separator()+info.baseName()+config_suffix);
        QFileInfo config(fileConfig);

        if(config.exists())
        {
            QFile file(fileConfig);

            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                ptr_server_settings = new server_settings(file.readAll(), false);
                file.close();

                isRead = ptr_server_settings->is_valid();

            }else{
                qCritical("Can't file open ('%s').", qUtf8Printable(config.fileName()));
                qCritical("Error: '%s'", qUtf8Printable(file.errorString()));
            }
        }else{
            qCritical("File '%s' does not exist!", qUtf8Printable(config.fileName()));

            fprintf(stderr, "Create new file settings: %s\n", qUtf8Printable(fileConfig));
            saveSettings(fileConfig);
        }
    }

    return isRead;
}

bool core_sweep::saveSettings(const QString &file)
{
    bool isSave(false);

    if(!file.isEmpty())
    {
        QFileInfo info(file);
        QString fileConfig(info.absolutePath()+QDir::separator()+info.baseName()+config_suffix);
        QFileInfo config(fileConfig);
        bool fileExists = config.exists();

        QFile file(fileConfig);

        if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            if(fileExists){
                if(ptr_server_settings)
                    file.write(ptr_server_settings->to_json());
            }else{
                const auto defaultSettings = server_settings();
                file.write(defaultSettings.to_json(false, false));
            }

            file.close();
            isSave = true;
        }else{
            qCritical("Can't file open ('%s').", qUtf8Printable(config.fileName()));
            qCritical("Error: '%s'", qUtf8Printable(file.errorString()));
        }
    }

    return isSave;
}

void core_sweep::initialization()
{    
    m_run_sweep_worker = false;

    ptr_sweep_topic = new sweep_topic(this);

    if(ptr_server_settings->spectrum_source_native())
        init_spectrum_native_worker();
    else
        init_spectrum_process_worker();

    // MQTT
    ptrMqttClient = new QMqttClient(this);

    // new style
    connect(ptrMqttClient, &QMqttClient::messageReceived,
            this, &core_sweep::slot_message_received);

    connect(ptrMqttClient, &QMqttClient::stateChanged,
            this, &core_sweep::updateLogStateChange);
    connect(ptrMqttClient, &QMqttClient::disconnected,
            this, &core_sweep::brokerDisconnected);
    connect(ptrMqttClient, &QMqttClient::connected,
            this, &core_sweep::pingReceived);
    connect(ptrMqttClient, &QMqttClient::pingResponseReceived,
            this , &core_sweep::pingReceived);
    connect(ptrMqttClient, &QMqttClient::errorChanged,
            this, &core_sweep::errorChanged);

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

    QTimer::singleShot(ptr_server_settings->delayed_launch(), this, &core_sweep::launching);
}

void core_sweep::launching()
{
    // connect to broker
    if(ptr_server_settings&&ptr_server_settings->is_valid())
    {
#ifdef QT_DEBUG
        qDebug().noquote() << tr("settings->") << tr("id:") << ptr_server_settings->id();
        qDebug().noquote() << tr("settings->") << tr("launching:") << ptr_server_settings->is_valid();
        qDebug().noquote() << tr("settings->") << tr("host broker:") << ptr_server_settings->host_broker();
        qDebug().noquote() << tr("settings->") << tr("port broker:") << ptr_server_settings->port_broker();
        qDebug().noquote() << tr("settings->") << tr("monitor interval:") << ptr_server_settings->system_monitor_interval();
        qDebug().noquote() << tr("settings->") << tr("delayed launch:") << ptr_server_settings->delayed_launch();
#endif

        // connect to MQTT broker
        if(ptrMqttClient)
        {
            ptrMqttClient->setHostname(ptr_server_settings->host_broker());
            ptrMqttClient->setPort(ptr_server_settings->port_broker());

            if (ptrMqttClient->state() == QMqttClient::Disconnected)
                ptrMqttClient->connectToHost();
        }
        // start system monitor
        ptrTimer->start(ptr_server_settings->system_monitor_interval());
    }
}

void core_sweep::updateLogStateChange()
{
    if (ptrMqttClient->state() == QMqttClient::Connected)
    {
        auto subscription = ptrMqttClient->subscribe(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_ctrl), 0);

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
             << ptrMqttClient->state();
#endif
}

void core_sweep::brokerDisconnected()
{
#ifdef QT_DEBUG
    qDebug() << "brokerDisconnected";
#endif
}

void core_sweep::pingReceived()
{
#ifdef QT_DEBUG
    qDebug() << "pingReceived";
#endif
}

void core_sweep::connecting()
{
#ifdef QT_DEBUG
    qDebug() << "connecting";
#endif
}

void core_sweep::errorChanged(QMqttClient::ClientError error)
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
