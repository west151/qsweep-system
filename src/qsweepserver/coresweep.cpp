#include "coresweep.h"
#include <QCoreApplication>
#include <QThread>
#include <QTimer>

#include <time.h>

#include "hackrfinfo.h"
#include "sweepworker.h"
#include "ctrlsweepworker.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

CoreSweep::CoreSweep(QObject *parent) : QObject(parent),
    ptrHackrfInfo(new HackrfInfo(this)),
    ptrSweepWorker(new SweepWorker),
    ptrSweepThread(new QThread),
    ptrCtrlSweepWorker(new CtrlSweepWorker(this)),
    ptrMqttClient(new QMqttClient(this))
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

#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO << topic.name() << ":" << ctrl;
#endif
}

void CoreSweep::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
            + QLatin1String(": State Change")
            + QString::number(ptrMqttClient->state());

    if (ptrMqttClient->state() == QMqttClient::Connected)
    {
        auto subscription = ptrMqttClient->subscribe(topic_ctrl, 0);

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

void CoreSweep::sendingMessage()
{

}
