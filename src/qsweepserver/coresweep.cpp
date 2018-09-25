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

static const QString config_suffix(QString(".config"));

CoreSweep::CoreSweep(const QString &file, QObject *parent) : QObject(parent),
    ptrHackrfInfo(new HackrfInfo(this)),
    ptrMqttClient(new QMqttClient(this)),
    ptrSweepTopic(new QSweepTopic(this)),
    ptrTimer(new QTimer(this))
{
    initialization();

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

            QTimer::singleShot(ptrSweepServerSettings->delayedLaunch(), this, &CoreSweep::launching);
        }else{
            qCritical("Can't file open ('%s').", qUtf8Printable(config.fileName()));
            qCritical("Error: '%s'", qUtf8Printable(file.errorString()));
        }
    }else{
        qCritical("File '%s' does not exist!", qUtf8Printable(config.fileName()));
    }
}

void CoreSweep::onDataFromWorker(const QByteArray &value)
{
    Q_UNUSED(value)
//#ifdef QT_DEBUG
//    qDebug() << Q_FUNC_INFO << ">" << value << "size" << value.size();
//#endif
}

void CoreSweep::initialization()
{
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

    // TEST
//    connect(SweepWorker::getInstance(), &SweepWorker::sendData,
//            this, &CoreSweep::onDataFromWorker);

    ptrSweepThread->start();

    // MQTT
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

    // HackRF Info
    connect(this, &CoreSweep::sendRunSweepInfo,
            ptrHackrfInfo, &HackrfInfo::onRunHackrfInfo);
    connect(ptrHackrfInfo, &HackrfInfo::sendHackrfInfo,
            this, &CoreSweep::sendingMessage);

    // System monitor
    ptrSystemMonitorWorker = new SystemMonitorWorker;
    ptrSystemMonitorThread = new QThread;
    ptrSystemMonitorWorker->moveToThread(ptrSystemMonitorThread);
    // start system monitor
    connect(ptrTimer, &QTimer::timeout,
            ptrSystemMonitorWorker, &SystemMonitorWorker::runSystemMonitorWorker);
    // send result
    connect(ptrSystemMonitorWorker, &SystemMonitorWorker::sendSystemMonitorResult,
            this, &CoreSweep::onSendingMessageRequest);

    ptrSystemMonitorThread->start();

//#ifdef QT_DEBUG
//    qDebug() << QThread::currentThreadId();
//    qDebug() << ptrSweepThread->currentThread();
//#endif
}

void CoreSweep::launching()
{
    // connect to broker
    if(ptrSweepServerSettings&&ptrSweepServerSettings->isValid())
    {
#ifdef QT_DEBUG
        qDebug() << tr("launching:") << ptrSweepServerSettings->isValid();
        qDebug() << tr("host broker:") << ptrSweepServerSettings->hostBroker();
        qDebug() << tr("port broker:") << ptrSweepServerSettings->portBroker();
        qDebug() << tr("monitor interval:") << ptrSweepServerSettings->systemMonitorInterval();
        qDebug() << tr("delayed launch:") << ptrSweepServerSettings->delayedLaunch();
#endif

        // connect to MQTT broker
        if(ptrMqttClient)
        {
            ptrMqttClient->setHostname(ptrSweepServerSettings->hostBroker());
            ptrMqttClient->setPort(ptrSweepServerSettings->portBroker());

            if (ptrMqttClient->state() == QMqttClient::Disconnected)
                ptrMqttClient->connectToHost();
        }
    }

    // start system monitor
    ptrTimer->start(ptrSweepServerSettings->systemMonitorInterval());
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
    const QString content = QDateTime::currentDateTime().toString()
            + QLatin1String(": State Change")
            + QString::number(ptrMqttClient->state());

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
    qDebug() << content;
#endif
}

void CoreSweep::brokerDisconnected()
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
}

void CoreSweep::pingReceived()
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
}

void CoreSweep::connecting()
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO;
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
