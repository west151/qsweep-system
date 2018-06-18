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

void CoreSweep::initialization()
{
    ptrSweepWorker = new SweepWorker;
    ptrSweepThread = new QThread;
    ptrSweepWorker->moveToThread(ptrSweepThread);

    connect(this, &CoreSweep::sendRunSweepWorker,
            ptrSweepWorker, &SweepWorker::onRunSweepWorker);

//    connect(ptrCtrlSweepWorker, &CtrlSweepWorker::sendStopSweepWorker,
//            ptrSweepWorker, &SweepWorker::onStopSweepWorker);

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

    QSweepRequest request(message, false);

    if(request.isValid()){
        switch (request.typeRequest()) {
        case TypeRequest::INFO:
            emit sendRunSweepInfo(message);
            break;
        case TypeRequest::SWEEP_SPECTR:
            emit sendRunSweepWorker(message);
            break;
        default:
            break;
        }

#ifdef QT_DEBUG
        qDebug() << "Server: TOPIC" << topic.name();
        qDebug() << "Server: JSON" << ctrl;
#endif
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
            qDebug() << Q_FUNC_INFO << tr("Data sending to host result:") << result;
#endif
        }
        break;
    default:
        break;
    }
}
