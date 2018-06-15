#include "coresweep.h"
#include <QCoreApplication>
#include <QThread>
#include <QTimer>

#include <time.h>

#include "hackrfinfo.h"
#include "sweepworker.h"
#include "ctrlsweepworker.h"
#include "qsweeptopic.h"
#include "qsweeprequest.h"
#include "qsweepanswer.h"


#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

CoreSweep::CoreSweep(QObject *parent) : QObject(parent),
    ptrHackrfInfo(new HackrfInfo(this)),
    ptrSweepWorker(new SweepWorker),
    ptrSweepThread(new QThread),
    ptrCtrlSweepWorker(new CtrlSweepWorker(this)),
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

void CoreSweep::initialization()
{
    ptrSweepWorker->moveToThread(ptrSweepThread);

    connect(ptrCtrlSweepWorker, &CtrlSweepWorker::sendRunSweepWorker,
            ptrSweepWorker, &SweepWorker::onRunSweepWorker);
    connect(ptrCtrlSweepWorker, &CtrlSweepWorker::sendStopSweepWorker,
            ptrSweepWorker, &SweepWorker::onStopSweepWorker);
    connect(ptrCtrlSweepWorker, &CtrlSweepWorker::sendParamsSweepWorker,
            ptrSweepWorker, &SweepWorker::onParamsSweepWorker);

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
    connect(ptrHackrfInfo, &HackrfInfo::sendHackrfInfo,
            this, &CoreSweep::sendingMessage);
}

void CoreSweep::launching()
{
    //QTimer::singleShot(5000, ptrCtrlSweepWorker, SLOT(startSweepWorkerTest()));

    // RX LNA (IF) gain, 0-40dB, 8dB steps
    // RX VGA (baseband) gain, 0-62dB, 2dB steps
    // ptrSweepWorker->runSweepWorker(2400, 2700, 500000, 40, 62);

    // connect to MQTT broker
    onConnectToHost("127.0.0.1", 1883);
}

void CoreSweep::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    QString ctrl(message);

    if(ctrl.contains("run")){
#ifdef QT_DEBUG
        qDebug() << Q_FUNC_INFO << tr("start");
#endif

        ptrCtrlSweepWorker->startSweepWorkerTest();
    }

    QSweepRequest request(message, false);

    if(request.isValid()){
        if(request.typeRequest() == TypeRequest::INFO){
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << "isValid:";
            qDebug() << Q_FUNC_INFO << topic.name();
            qDebug() << Q_FUNC_INFO << ctrl;
#endif
            ptrHackrfInfo->getHackrfInfo();
        }
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
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << tr("Data sending to host result:") << result << "test";
#endif
        }
        break;
    default:
        break;
    }
}
