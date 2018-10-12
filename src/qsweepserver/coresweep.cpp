#include "coresweep.h"
#include <QThread>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "hackrfinfo.h"
#include "sweepworker.h"
#include "qsweeptopic.h"
#include "qsweeprequest.h"
#include "qsweepanswer.h"
#include "qsweepparams.h"

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
    ptrSweepTopic = new QSweepTopic(this);

    ptrSweepWorker = new SweepWorker;
    ptrSweepThread = new QThread;
    ptrSweepWorker->moveToThread(ptrSweepThread);

    connect(this, &CoreSweep::sendRunSweepWorker,
            ptrSweepWorker, &SweepWorker::onRunSweepWorker);
    connect(ptrSweepWorker, &SweepWorker::sendSweepWorkerMessagelog,
            this, &CoreSweep::onSendingMessageRequest);
    // Power spectr
    connect(SweepWorker::getInstance(), &SweepWorker::sendPowerSpectr,
            this, &CoreSweep::onSendingMessageRequest);
    // stop spectr
    connect(this, &CoreSweep::sendStopSweepWorker,
            SweepWorker::getInstance(), &SweepWorker::onStopSweepWorker);

    ptrSweepThread->start();

    // MQTT
    ptrMqttClient = new QMqttClient(this);
    connect(ptrMqttClient, &QMqttClient::messageReceived,
            this, &CoreSweep::messageReceived);
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
    connect(this, &CoreSweep::sendRunSweepInfo,
            ptrHackrfInfo, &HackrfInfo::onRunHackrfInfo);
    connect(ptrHackrfInfo, &HackrfInfo::sendHackrfInfo,
            this, &CoreSweep::sendingMessage);

    // System monitor
    ptrSystemMonitorWorker = new SystemMonitorWorker;
    ptrSystemMonitorThread = new QThread;
    ptrSystemMonitorWorker->moveToThread(ptrSystemMonitorThread);

    // start system monitor
    ptrTimer = new QTimer(this);
    connect(ptrTimer, &QTimer::timeout,
            ptrSystemMonitorWorker, &SystemMonitorWorker::runSystemMonitorWorker);
    // send result
    connect(ptrSystemMonitorWorker, &SystemMonitorWorker::sendSystemMonitorResult,
            this, &CoreSweep::onSendingMessageRequest);

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

void CoreSweep::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    switch (ptrSweepTopic->sweepTopic(topic.name())) {
    case QSweepTopic::TOPIC_CTRL:
    {
        QSweepRequest request(message, false);

        if(request.isValid()){
            switch (request.typeRequest()) {
            case TypeRequest::INFO:
                emit sendRunSweepInfo(message);
                break;
            case TypeRequest::START_SWEEP_SPECTR:
                emit sendRunSweepWorker(message);
                break;
            case TypeRequest::STOP_SWEEP_SPECTR:
                emit sendStopSweepWorker();
                break;
            default:
                break;
            }
        }
    }
        break;
    default:
        break;
    }
}

void CoreSweep::updateLogStateChange()
{
    if (ptrMqttClient->state() == QMqttClient::Connected)
    {
        auto subscription = ptrMqttClient->subscribe(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_CTRL), 0);

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

void CoreSweep::sendingMessage(const QSweepAnswer &value)
{
    switch (value.typeAnswer()) {
    case TypeAnswer::INFO:
        if (ptrMqttClient->state() == QMqttClient::Connected) {
            qint32 result = ptrMqttClient->publish(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_INFO), value.exportToJson());

            Q_UNUSED(result)
        }
        break;
    default:
        break;
    }
}

void CoreSweep::onSendingMessageRequest(const QByteArray &value)
{
    if (ptrMqttClient->state() == QMqttClient::Connected)
    {
        const QSweepAnswer answer(value, false);

        switch (answer.typeAnswer()) {
        case TypeAnswer::INFO:
        {
            ptrMqttClient->publish(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_INFO), answer.exportToJson());
        }
            break;
        case TypeAnswer::SWEEP_MESSAGE_LOG:
        {
            ptrMqttClient->publish(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_MESSAGE_LOG), answer.exportToJson());
        }
            break;
        case TypeAnswer::SWEEP_POWER_SPECTR:
        {
            ptrMqttClient->publish(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_POWER_SPECTR), answer.exportToJson());
        }
            break;
        case TypeAnswer::SWEEP_SYSTEM_MONITOR:
        {
            ptrMqttClient->publish(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_SYSTEM_MONITOR), answer.exportToJson());
        }
            break;
        default:
            break;
        }

//#ifdef QT_DEBUG
//            qDebug() << Q_FUNC_INFO << tr("Answer JSON:") << answer.exportToJson();
//#endif
    }
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
    }
}
