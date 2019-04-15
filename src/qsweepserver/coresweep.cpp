#include "coresweep.h"
#include <QThread>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "hackrfinfo.h"
#include "sweepworker.h"
#include "sweep_topic.h"

#include "sweep_message.h"
#include "params_spectr.h"

static const QString config_suffix(QString(".conf"));

CoreSweep::CoreSweep(const QString &file, QObject *parent) : QObject(parent)
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

void CoreSweep::slot_publish_message(const QByteArray &value)
{
    if (ptrMqttClient->state() == QMqttClient::Connected)
    {
        const sweep_message send_data(value);

        if(send_data.is_valid())
        {
            if(send_data.type() == type_message::data_sdr_info)
                ptrMqttClient->publish(ptrSweepTopic->sweep_topic_by_type(sweep_topic::TOPIC_INFO), value);

            if(send_data.type() == type_message::data_message_log)
                ptrMqttClient->publish(ptrSweepTopic->sweep_topic_by_type(sweep_topic::TOPIC_MESSAGE_LOG), value);

            if(send_data.type() == type_message::data_system_monitor)
                ptrMqttClient->publish(ptrSweepTopic->sweep_topic_by_type(sweep_topic::TOPIC_SYSTEM_MONITOR), value);

            if(send_data.type() == type_message::data_spectr)
                ptrMqttClient->publish(ptrSweepTopic->sweep_topic_by_type(sweep_topic::TOPIC_POWER_SPECTR), value);
        }
    }
}

void CoreSweep::slot_message_received(const QByteArray &message, const QMqttTopicName &topic)
{
    if(ptrSweepTopic->sweep_topic_by_str(topic.name()) == sweep_topic::TOPIC_CTRL)
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

                if(params_spectr_data.start_spectr()){
                    if(!m_run_sweep_worker)
                        emit signal_run_spectr_worker(message);

                    if(m_run_sweep_worker){
                        emit signal_stop_spectr_worker();
                        emit signal_run_spectr_worker(message);
                    }
                }else
                    emit signal_stop_spectr_worker();
            }
        }
    }
}

void CoreSweep::slot_sweep_worker(const bool &on)
{
    m_run_sweep_worker = on;
}

bool CoreSweep::readSettings(const QString &file)
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
                ptrSweepServerSettings = new SweepServerSettings(file.readAll(), false);
                file.close();

                isRead = ptrSweepServerSettings->isValid();

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

bool CoreSweep::saveSettings(const QString &file)
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
                if(ptrSweepServerSettings)
                    file.write(ptrSweepServerSettings->exportToJson());
            }else{
                const auto defaultSettings = SweepServerSettings();
                file.write(defaultSettings.exportToJson(false, false));
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

void CoreSweep::initialization()
{    
    m_run_sweep_worker = false;

    ptrSweepTopic = new sweep_topic(this);

    ptrSweepWorker = new SweepWorker;
    ptrSweepThread = new QThread;
    ptrSweepWorker->moveToThread(ptrSweepThread);

    connect(this, &CoreSweep::signal_run_spectr_worker,
            ptrSweepWorker, &SweepWorker::slot_run_sweep_worker);

    connect(ptrSweepWorker, &SweepWorker::signal_sweep_message,
            this, &CoreSweep::slot_publish_message);

    connect(ptrSweepWorker, &SweepWorker::signal_sweep_worker,
            this, &CoreSweep::slot_sweep_worker);

    // Power spectr
    connect(SweepWorker::getInstance(), &SweepWorker::signal_sweep_message,
            this, &CoreSweep::slot_publish_message);
    // stop spectr
    connect(this, &CoreSweep::signal_stop_spectr_worker,
            SweepWorker::getInstance(), &SweepWorker::slot_stop_sweep_worker);


    ptrSweepThread->start();

    // MQTT
    ptrMqttClient = new QMqttClient(this);

    // new style
    connect(ptrMqttClient, &QMqttClient::messageReceived,
            this, &CoreSweep::slot_message_received);


    connect(ptrMqttClient, &QMqttClient::stateChanged,
            this, &CoreSweep::updateLogStateChange);
    connect(ptrMqttClient, &QMqttClient::disconnected,
            this, &CoreSweep::brokerDisconnected);
    connect(ptrMqttClient, &QMqttClient::connected,
            this, &CoreSweep::pingReceived);
    connect(ptrMqttClient, &QMqttClient::pingResponseReceived,
            this , &CoreSweep::pingReceived);
    connect(ptrMqttClient, &QMqttClient::errorChanged,
            this, &CoreSweep::errorChanged);

    // HackRF Info
    ptrHackrfInfo = new HackrfInfo(this);
    // get hacrf info
    connect(this, &CoreSweep::signal_run_hackrf_info,
            ptrHackrfInfo, &HackrfInfo::slot_run_hackrf_info);
    // send result to broker
    connect(ptrHackrfInfo, &HackrfInfo::signal_hackrf_info,
            this, &CoreSweep::slot_publish_message);

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
            this, &CoreSweep::slot_publish_message);

    ptrSystemMonitorThread->start();

    QTimer::singleShot(ptrSweepServerSettings->delayedLaunch(), this, &CoreSweep::launching);
}

void CoreSweep::launching()
{
    // connect to broker
    if(ptrSweepServerSettings&&ptrSweepServerSettings->isValid())
    {
#ifdef QT_DEBUG
        qDebug().noquote() << tr("settings->") << tr("id:") << ptrSweepServerSettings->id();
        qDebug().noquote() << tr("settings->") << tr("launching:") << ptrSweepServerSettings->isValid();
        qDebug().noquote() << tr("settings->") << tr("host broker:") << ptrSweepServerSettings->hostBroker();
        qDebug().noquote() << tr("settings->") << tr("port broker:") << ptrSweepServerSettings->portBroker();
        qDebug().noquote() << tr("settings->") << tr("monitor interval:") << ptrSweepServerSettings->systemMonitorInterval();
        qDebug().noquote() << tr("settings->") << tr("delayed launch:") << ptrSweepServerSettings->delayedLaunch();
#endif

        // connect to MQTT broker
        if(ptrMqttClient)
        {
            ptrMqttClient->setHostname(ptrSweepServerSettings->hostBroker());
            ptrMqttClient->setPort(ptrSweepServerSettings->portBroker());

            if (ptrMqttClient->state() == QMqttClient::Disconnected)
                ptrMqttClient->connectToHost();
        }
        // start system monitor
        ptrTimer->start(ptrSweepServerSettings->systemMonitorInterval());
    }
}

void CoreSweep::updateLogStateChange()
{
    if (ptrMqttClient->state() == QMqttClient::Connected)
    {
        auto subscription = ptrMqttClient->subscribe(ptrSweepTopic->sweep_topic_by_type(sweep_topic::TOPIC_CTRL), 0);

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

void CoreSweep::brokerDisconnected()
{
#ifdef QT_DEBUG
    qDebug() << "brokerDisconnected";
#endif
}

void CoreSweep::pingReceived()
{
#ifdef QT_DEBUG
    qDebug() << "pingReceived";
#endif
}

void CoreSweep::connecting()
{
#ifdef QT_DEBUG
    qDebug() << "connecting";
#endif
}

void CoreSweep::errorChanged(QMqttClient::ClientError error)
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
