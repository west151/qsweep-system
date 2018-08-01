#include "coresweep.h"
#include <QCoreApplication>
#include <QThread>
#include <QTimer>

#include <time.h>

#include "hackrfinfo.h"
#include "sweepworker.h"
#include "qsweeptopic.h"
#include "qsweeprequest.h"
#include "qsweepanswer.h"
#include "qsweepparams.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

CoreSweep::CoreSweep(QObject *parent) : QObject(parent),
    ptrHackrfInfo(new HackrfInfo(this)),
    ptrMqttClient(new QMqttClient(this)),
    ptrSweepTopic(new QSweepTopic(this))
{
}

int CoreSweep::runCoreSweep(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    initialization();

    launching();

    return app.exec();
}

void CoreSweep::onConnectToHost(const QString &host, const quint16 &port)
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO << tr("Connect to host:") << host << tr("port:") << port;
#endif

    if(ptrMqttClient)
    {
        ptrMqttClient->setHostname(host);
        ptrMqttClient->setPort(port);

        if (ptrMqttClient->state() == QMqttClient::Disconnected)
            ptrMqttClient->connectToHost();
    }
}

void CoreSweep::onDataFromWorker(const QByteArray &value)
{
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

#ifdef QT_DEBUG
    qDebug() << QThread::currentThreadId();
    qDebug() << ptrSweepThread->currentThread();
#endif
}

void CoreSweep::launching()
{
    // connect to MQTT broker
    onConnectToHost("127.0.0.1", 1883);
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
            case TypeRequest::SWEEP_SPECTR:
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
            qDebug() << Q_FUNC_INFO << "Could not subscribe. Is there a valid connection?";
#endif
            return;
        }
    }

#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO << content;
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
        default:
            break;
        }

//#ifdef QT_DEBUG
//            qDebug() << Q_FUNC_INFO << tr("Answer JSON:") << answer.exportToJson();
//#endif
    }
}
