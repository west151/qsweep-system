#include "core_sweep_client.h"

#include <QQmlContext>
#include <QFileInfo>
#include <QDir>
//#include <QTime>
#include <QtCore/qdebug.h>
#include <QThread>
#include <QStandardPaths>
#include <QNetworkInterface>

#include "user_interface.h"
#include "sweep_topic.h"
#include "chart/surface_spectr.h"
#include "system_monitor_interface.h"
#include "state_sweep_client.h"
#include "settings/client_settings.h"
#include "template/ranges_template.h"

#include "sweep_message.h"
#include "sdr_info.h"
#include "data_log.h"
#include "system_monitor.h"
#include "data_spectr.h"
#include "spectr/ta_spectr.h"
#include "database/db_local_state.h"

static const QString str_path_template(QString("template"));
static const QString str_template_suffix(QString(".template"));

core_sweep_client::core_sweep_client(const QString &path_config, const QString &app_name, QObject *parent) : QObject(parent)

{
    qInfo("Init core ...");
    qInfo("Path config: '%s'", qUtf8Printable(path_config));
    m_path_config = path_config;

    QDir dir(path_config);
    if (!dir.exists())
        dir.mkpath(".");

    m_file_settings = path_config + QDir::separator() + app_name + ".conf";

    // create new file config
    QFileInfo file_config(m_file_settings);
    if(!file_config.exists())
        save_settings();

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

    ptr_sweep_topic = new sweep_topic(this);

//    m_timer_receive = new QTimer;

    qRegisterMetaType<data_spectr>();
    qRegisterMetaType<QVector<params_spectr> >();
    qRegisterMetaType<ranges_template>();
}

bool core_sweep_client::read_settings()
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
                ptr_client_settings = new client_settings(file.readAll());
                file.close();

                result = ptr_client_settings->is_valid();

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

void core_sweep_client::initialization(QQmlApplicationEngine *ptr_engine)
{
    // MQTT
    ptr_mqtt_client = new QMqttClient(this);
    connect(ptr_mqtt_client, &QMqttClient::messageReceived,
            this, &core_sweep_client::slot_message_received);
    connect(ptr_mqtt_client, &QMqttClient::stateChanged,
            this, &core_sweep_client::updateLogStateChange);
    connect(ptr_mqtt_client, &QMqttClient::disconnected,
            this, &core_sweep_client::brokerDisconnected);
    connect(ptr_mqtt_client, &QMqttClient::connected,
            this, &core_sweep_client::pingReceived);
    connect(ptr_mqtt_client, &QMqttClient::pingResponseReceived,
            this , &core_sweep_client::pingReceived);
    connect(ptr_mqtt_client, &QMqttClient::errorChanged,
            this, &core_sweep_client::errorChanged);

    // System Monitor Interface
    ptr_system_monitor_interface = new SystemMonitorInterface;
    connect(this, &core_sweep_client::signal_system_monitor,
            ptr_system_monitor_interface, &SystemMonitorInterface::slot_system_monitor);

    // user interface
    ptr_user_interface = new user_interface;
    connect(ptr_user_interface, &user_interface::signal_sweep_message,
            this, &core_sweep_client::slot_publish_message);

    // StateSweepClient
    ptr_state_sweep_client = new StateSweepClient(this);
    connect(this, &core_sweep_client::sendStateConnected,
            ptr_state_sweep_client, &StateSweepClient::onConnect);
    connect(this, &core_sweep_client::sendStateConnected,
            ptr_user_interface, &user_interface::onRequestSweepInfo);
    connect(this, &core_sweep_client::sendStateDisconnected,
            ptr_state_sweep_client, &StateSweepClient::onDisconnect);

    // Hackrf Info Model
    ptr_hackrf_info_model = new hackrf_info_model(this);
    connect(this, &core_sweep_client::signal_sdr_info,
            ptr_hackrf_info_model, &hackrf_info_model::add_result);

    // Message Log Model
    ptr_message_log_model = new message_log_model(this);
    connect(this, &core_sweep_client::signal_data_log,
            ptr_message_log_model, &message_log_model::add_result);

    // database local
    ptr_db_local_state_worker = new db_local_state;
    ptr_db_local_thread = new QThread;
    ptr_db_local_state_worker->moveToThread(ptr_db_local_thread);

    connect(this, &core_sweep_client::signal_open_db,
            ptr_db_local_state_worker, &db_local_state::slot_open_db);
    connect(ptr_user_interface, &user_interface::signal_read_params_spectr,
            ptr_db_local_state_worker, &db_local_state::slot_read_params_spectr);
    // save params
    connect(ptr_user_interface, &user_interface::signal_save_params_spectr,
            ptr_db_local_state_worker, &db_local_state::slot_write_params_spectr);

    // params spectr model
    ptr_params_spectr_model = new params_spectr_model(this);
    connect(ptr_db_local_state_worker, &db_local_state::signal_read_params_spectr,
            ptr_params_spectr_model, &params_spectr_model::slot_set_vector_result);

    // send params from model
    connect(ptr_params_spectr_model, &params_spectr_model::signal_params_from_model,
            ptr_user_interface, &user_interface::slot_set_params_spectr);

    // sync db & model
    connect(ptr_params_spectr_model, &params_spectr_model::signal_remove_from_model,
            ptr_db_local_state_worker, &db_local_state::slot_remove_params_spectr);

    ptr_db_local_thread->start();

    // ranges template model
    ptr_ranges_template_model = new ranges_template_model(this);
    connect(this, &core_sweep_client::signal_ranges_template,
            ptr_ranges_template_model, &ranges_template_model::add_result);

    // GUI
    qmlRegisterType<surface_spectr>("surfacespectr", 1, 0, "SurfaceSpectr");
    ptr_engine->rootContext()->setContextProperty("userInterface", ptr_user_interface);
    ptr_engine->rootContext()->setContextProperty("hackrfInfoModel", ptr_hackrf_info_model);
    ptr_engine->rootContext()->setContextProperty("messageLogModel", ptr_message_log_model);
    ptr_engine->rootContext()->setContextProperty("systemMonitorInterface", ptr_system_monitor_interface);
    ptr_engine->rootContext()->setContextProperty("stateSweepClient", ptr_state_sweep_client);
    ptr_engine->rootContext()->setContextProperty("paramsSpectrModel", ptr_params_spectr_model);
    ptr_engine->rootContext()->setContextProperty("rangesTemplateModel", ptr_ranges_template_model);

    ptr_engine->load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    QObject *rootObject = ptr_engine->rootObjects().first();

    // Spect surface
    QObject *qmlSurfaceSpectr = rootObject->findChild<QObject*>("objSurfaceSpectr");
    surface_spectr *surfaceSpectr = static_cast<surface_spectr *>(qmlSurfaceSpectr);

    connect(ptr_user_interface, &user_interface::signal_spectr_max_calc,
            surfaceSpectr, &surface_spectr::slot_spectr_max_calc);

    // ta spectr
    ptr_ta_spectr_worker = new ta_spectr;
    ptr_ta_spectr_thread = new QThread;
    ptr_ta_spectr_worker->moveToThread(ptr_ta_spectr_thread);

    connect(this, &core_sweep_client::signal_data_spectr,
            ptr_ta_spectr_worker, &ta_spectr::slot_data_spectr);

    connect(ptr_ta_spectr_worker, &ta_spectr::signal_spectr_rt,
            surfaceSpectr, &surface_spectr::slot_power_spectr);

    ptr_ta_spectr_thread->start();

    // readtemplate
    read_template(m_path_config);

//    if (engine.rootObjects().isEmpty())
//            return -1;
}

void core_sweep_client::start_client()
{
    // connect to mqtt broker
    if(ptr_mqtt_client)
    {
        ptr_mqtt_client->setHostname(ptr_user_interface->sweepClientSettings().host_broker());
        ptr_mqtt_client->setPort(ptr_user_interface->sweepClientSettings().port_broker());

        if (ptr_mqtt_client->state() == QMqttClient::Disconnected)
            ptr_mqtt_client->connectToHost();
    }

//    // start timer
//    if(m_timer_receive)
//        m_timer_receive->start();
//    m_size_data_receive = 0;

    // open/create database
    emit signal_open_db(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
}

void core_sweep_client::onDisconnectFromHost()
{
    if (ptr_mqtt_client->state() == QMqttClient::Connected){
        //ptrMqttClient->unsubscribe(ptrSweepTopic->sweep_topic_by_type(sweep_topic::TOPIC_DATA));
        ptr_mqtt_client->disconnectFromHost();
    }
}

bool core_sweep_client::read_template(const QString &path)
{
    bool result(false);

    if(!path.isEmpty())
    {
        const QFileInfo info(path);
        const QDir directory(info.absolutePath() + QDir::separator() + str_path_template);
        QStringList file_list = directory.entryList(QStringList() << "*" + str_template_suffix, QDir::Files);

        for(int i=0; i<file_list.size(); ++i)
        {
            QString file = info.absolutePath()
                    + QDir::separator()
                    + str_path_template
                    + QDir::separator()
                    + file_list.at(i);

            QFile file_renge(file);

            if(file_renge.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qInfo("File templete: '%s'", qUtf8Printable(file));

                const ranges_template range(file_renge.readAll());
                file_renge.close();

                if(range.is_valid())
                    emit signal_ranges_template(range);

            }else{
                qCritical("Can't file open ('%s').", qUtf8Printable(file_renge.fileName()));
                qCritical("Error: '%s'", qUtf8Printable(file_renge.errorString()));
            }
        }
    }
    return result;
}

void core_sweep_client::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
            + QLatin1String(": State Change ")
            + QString::number(ptr_mqtt_client->state());

    qInfo("State change: '%s'", qUtf8Printable(content));

    // Subscribers topic
    if (ptr_mqtt_client->state() == QMqttClient::Connected)
    {
        auto subscription = ptr_mqtt_client->subscribe(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_info));

        if (!subscription)
        {
            qInfo("Could not subscribe. Is there a valid connection ?");
            return;
        }

        auto subscription1 = ptr_mqtt_client->subscribe(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_message_log));

        if (!subscription1)
        {
            qInfo("Could not subscribe. Is there a valid connection ?");
            return;
        }

        auto subscription2 = ptr_mqtt_client->subscribe(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_power_spectr));

        if (!subscription2)
        {
            qInfo("Could not subscribe. Is there a valid connection ?");
            return;
        }

        auto subscription3 = ptr_mqtt_client->subscribe(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_system_monitor));

        if (!subscription3)
        {
            qInfo("Could not subscribe. Is there a valid connection ?");
            return;
        }

        emit sendStateConnected();
    }

    if (ptr_mqtt_client->state() == QMqttClient::Disconnected)
    {
        emit sendStateDisconnected();
    }
}

void core_sweep_client::brokerDisconnected()
{
    qInfo("Broker disconnected ...");
}

void core_sweep_client::pingReceived()
{
    ptr_user_interface->onPingReceived();

    qInfo("Ping received ...");
}

void core_sweep_client::connecting()
{
    qInfo("Connecting ...");
}

void core_sweep_client::slot_publish_message(const QByteArray &value)
{
    if(ptr_mqtt_client)
    {
        if (ptr_mqtt_client->state() == QMqttClient::Connected)
        {
            const sweep_message send_data(value);

            if(send_data.is_valid())
            {
                QString msg;

                if(send_data.type() == type_message::ctrl_info)
                {
                    qint32 id = ptr_mqtt_client->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_ctrl), send_data.to_json());
                    msg.append("Mqtt public: ").append(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_ctrl));
                    msg.append(" id: ").append(QString::number(id));
                    qInfo("'%s'",qUtf8Printable(msg));
                }
                if(send_data.type() == type_message::ctrl_spectr)
                {
                    qint32 id = ptr_mqtt_client->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_ctrl), send_data.to_json());
                    msg.append("Mqtt public: ").append(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_ctrl));
                    msg.append(" id: ").append(QString::number(id));
                    qInfo("'%s'",qUtf8Printable(msg));
                }
                if(send_data.type() == type_message::ctrl_db)
                {
                    qint32 id = ptr_mqtt_client->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_db_ctrl), send_data.to_json());
                    msg.append("Mqtt public: ").append(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_db_ctrl));
                    msg.append(" id: ").append(QString::number(id));
                    qInfo("'%s'",qUtf8Printable(msg));
                }
            } else {
                qWarning("Can't send (publish) data to host ...");
            }
            qInfo("Data sending (publish) to host ...");
            // qInfo("Data: '%s'", qUtf8Printable(QString(send_data.to_json())));
        }
    }
}

void core_sweep_client::slot_message_received(const QByteArray &message, const QMqttTopicName &topic)
{
//    qInfo("slot_message_received: '%s'", qUtf8Printable(topic.name()));

    // system info
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_info)
    {
        const sweep_message data_received(message);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::data_sdr_info)
            {
                const sdr_info sdr_info_data(data_received.data_message());
                emit signal_sdr_info(sdr_info_data);
            }
        }
    }

    // message log
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_message_log)
    {
        const sweep_message data_received(message);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::data_message_log)
            {
                const data_log data_log_data(data_received.data_message());
                emit signal_data_log(data_log_data);
            }
        }
    }

    // system monitor
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_system_monitor)
    {
        const sweep_message data_received(message);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::data_system_monitor)
            {
                const system_monitor data_system_monitor(data_received.data_message());
                emit signal_system_monitor(data_system_monitor);
            }
        }
    }

    // power spectr
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_power_spectr)
    {
        const sweep_message data_received(message);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::data_spectr)
            {
                const data_spectr powers(data_received.data_message());

                emit signal_data_spectr(powers);
            }
        }
    }

//    m_size_data_receive = m_size_data_receive + message.size();

//    if(m_timer_receive->interval() >= 5000)
//    {
//        QString msg;
//        msg.append(topic.name());
//        msg.append(" Data size: ");
//        msg.append(QString::number(m_size_data_receive/1024.0, 'f', 2));
//        msg.append(" Kbyte ");
//        msg.append("( " ).append(QString::number((m_size_data_receive/1024)/1024.0, 'f', 2)).append(" Mbyte ) ");
//        msg.append(QString("Time elapsed: %1 ms, %2 s").arg(m_timer_receive->interval()).arg(m_timer_receive->interval()/1000));

//        QString msg("5ttergfhjd");
//        qInfo("Message received: '%s'", qUtf8Printable(msg));

//        m_timer_receive->start();
//        m_size_data_receive = 0;
//    }
}

bool core_sweep_client::save_settings()
{
    bool is_save(false);

    if(!m_file_settings.isEmpty())
    {
        QFileInfo file_config(m_file_settings);
        bool fileExists = file_config.exists();

        QFile file(m_file_settings);

        if(file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            if(fileExists)
            {
                if(ptr_client_settings)
                    file.write(ptr_client_settings->to_json());
            } else {
                auto defaultSettings = client_settings();
                QStringList local_address = get_local_address_v4();
                if(!local_address.isEmpty())
                    defaultSettings.set_host_broker(local_address.at(0));
                // set default ip
                file.write(defaultSettings.to_json());
            }

            file.close();
            is_save = true;
        }else{
            qCritical("Can't file open ('%s').", qUtf8Printable(m_file_settings));
            qCritical("Error: '%s'", qUtf8Printable(file.errorString()));
        }
    }

    return is_save;
}

void core_sweep_client::errorChanged(QMqttClient::ClientError error)
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

QStringList core_sweep_client::get_local_address_v4()
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
