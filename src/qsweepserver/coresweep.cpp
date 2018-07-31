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



//    switch (ptrSweepTopic->sweepTopic(topic.name())) {
//    case QSweepTopic::TOPIC_INFO:
//    {
//        QSweepAnswer answer(message);
//        const QHackrfInfo info(answer.dataAnswer(), false);

//        if(info.isValid()){
//            ptrHackrfInfoModel->addResult(info);

////#ifdef QT_DEBUG
////            qDebug() << "---------------------------------------------------";
////            qDebug() << Q_FUNC_INFO << tr("Type Answer:") << static_cast<qint32>(answer.typeAnswer());
////            qDebug() << Q_FUNC_INFO << tr("Index Board:") << info.indexBoard();
////            qDebug() << Q_FUNC_INFO << tr("Serial Numbers:") << info.serialNumbers();
////            qDebug() << Q_FUNC_INFO << tr("Board ID Number:") << info.boardID();
////            qDebug() << Q_FUNC_INFO << tr("Firmware Version:") << info.firmwareVersion();
////            qDebug() << Q_FUNC_INFO << tr("Part ID Number:") << info.partIDNumber();
////            qDebug() << Q_FUNC_INFO << tr("Libhackrf Version:") << info.libHackrfVersion();
////            qDebug() << Q_FUNC_INFO << tr("Size message (byte):") << message.size();
////#endif
//        }
//    }
//        break;
//    case QSweepTopic::TOPIC_MESSAGE_LOG:
//    {
//        QSweepAnswer answer(message);
//        QSweepMessageLog log(answer.dataAnswer());
//        ptrMessageLogModel->addResult(log);

////#ifdef QT_DEBUG
////        qDebug() << "---------------------------------------------------";
////        qDebug() << Q_FUNC_INFO << tr("DateTime:") << log.dateTime();
////        qDebug() << Q_FUNC_INFO << tr("Message Log:") << log.textMessage();
////#endif
//    }
//        break;
//    case QSweepTopic::TOPIC_POWER_SPECTR:
//    {
//        QSweepAnswer answer(message);
//        QSweepSpectr powers(answer.dataAnswer());

//        QVector<PowerSpectr> tmpPowerSpectr(powers.powerSpectr());

//        std::sort(tmpPowerSpectr.begin(), tmpPowerSpectr.end(), [](const PowerSpectr& a, const PowerSpectr& b) {
//            return a.m_frequency_min < b.m_frequency_min;
//        });

//        // for test
//        ptrDataSource->updateDate(2400, 2500, tmpPowerSpectr);
//        // update axis max & min
//        ptrAxisY->setMin(ptrDataSource->minValue());
//        ptrAxisY->setMax(ptrDataSource->maxValue());
//        // test
//        emit sendStartSpectr();

////#ifdef QT_DEBUG
////        qDebug() << "---------------------------------------------------";
////        qDebug() << tmpPowerSpectr.count() << tmpPowerSpectr.count()/4;

////        for(int i=0; i<tmpPowerSpectr.count(); ++i){
////            qDebug() << tmpPowerSpectr.at(i).m_frequency_min
////                     << tmpPowerSpectr.at(i).m_frequency_max
////                     << tmpPowerSpectr.at(i).m_fft_bin_width
////                     << tmpPowerSpectr.at(i).m_fft_size
////                     << tmpPowerSpectr.at(i).m_power;
////        }
////#endif
//    }
//        break;
//    default:
//        break;
//    }

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
