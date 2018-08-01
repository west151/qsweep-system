#include "coresweepclient.h"

#include <QApplication>
#include <QQmlContext>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <QLineSeries>
#include <QChartView>

#include "userinterface.h"
#include "qsweeptopic.h"
#include "qsweeprequest.h"
#include "qsweepanswer.h"
#include "qsweepspectr.h"
#include "qhackrfinfo.h"
#include "qsweepmessagelog.h"
#include "chart/datasource.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

static const QString config_suffix(QString(".json"));

CoreSweepClient::CoreSweepClient(QObject *parent) : QObject(parent),
    ptrUserInterface(new UserInterface(this)),
    ptrMqttClient(new QMqttClient(this)),
    ptrSweepTopic(new QSweepTopic(this)),
    ptrHackrfInfoModel(new HackrfInfoModel(this)),
    ptrMessageLogModel(new MessageLogModel(this)),
    ptrDataSource(new DataSource(this)),
    ptrAxisX(new QValueAxis(this)),
    ptrAxisY(new QValueAxis(this))
{
}

int CoreSweepClient::runCoreSweepClient(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("CoreSweepClient");
    QCoreApplication::setApplicationVersion("1.0");

    QApplication app(argc, argv);

    initialization();

    if(readSettings(app.applicationFilePath())){
        launching();
    }

    ptrEngine = new QQmlApplicationEngine(this);

    // Настройка осей графика
    ptrAxisX = new QValueAxis(this);
    ptrAxisX->setTitleText("Freq");
    ptrAxisX->setMin(2400);
    ptrAxisX->setMax(2500);
//    axisX->setLabelFormat("%i");
//    axisX->setTickCount(1);

    ptrAxisY = new QValueAxis(this);
    ptrAxisY->setTitleText("Level");
    ptrAxisY->setMin(ptrDataSource->minValue());
    ptrAxisY->setMax(ptrDataSource->maxValue());
//    axisY->setLabelFormat("%g");
//    axisY->setTickCount(5);

    QQmlContext *context = ptrEngine->rootContext();
    context->setContextProperty("userInterface", ptrUserInterface);
    context->setContextProperty("hackrfInfoModel", ptrHackrfInfoModel);
    context->setContextProperty("messageLogModel", ptrMessageLogModel);
    context->setContextProperty("dataSource", ptrDataSource);
    context->setContextProperty("valueAxisY", ptrAxisY);
    context->setContextProperty("valueAxisX", ptrAxisX);
    ptrEngine->load(QUrl(QLatin1String("qrc:/main.qml")));

    QObject *rootObject = ptrEngine->rootObjects().first();
    QObject *qmlChartView = rootObject->findChild<QObject*>("chartViewSpectr");
    qmlChartView->setProperty("title", tr("Signals"));

    //QObject *qmlLineSeriesPower = rootObject->findChild<QObject*>("lineSeriesPower");

    if (ptrEngine->rootObjects().isEmpty())
        return -1;

    return app.exec();
}

void CoreSweepClient::onConnectToHost(const QString &host, const quint16 &port)
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

void CoreSweepClient::onDisconnectFromHost()
{
    if (ptrMqttClient->state() == QMqttClient::Connected){
        ptrMqttClient->unsubscribe(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_DATA));
        ptrMqttClient->disconnectFromHost();
    }
}

void CoreSweepClient::initialization()
{
    // MQTT
    connect(ptrMqttClient, &QMqttClient::messageReceived,
            this, &CoreSweepClient::messageReceived);
    connect(ptrMqttClient, &QMqttClient::stateChanged,
            this, &CoreSweepClient::updateLogStateChange);
    connect(ptrMqttClient, &QMqttClient::disconnected,
            this, &CoreSweepClient::brokerDisconnected);
    connect(ptrMqttClient, &QMqttClient::connected,
            this, &CoreSweepClient::pingReceived);
    connect(ptrMqttClient, &QMqttClient::pingResponseReceived,
            this , &CoreSweepClient::pingReceived);
    connect(ptrUserInterface, &UserInterface::sendRequestSweepServer,
            this, &CoreSweepClient::sendingRequest);
    connect(this, &CoreSweepClient::sendStartSpectr,
            ptrUserInterface, &UserInterface::sendStartSpectr);
}

bool CoreSweepClient::readSettings(const QString &file) const
{
    QFileInfo info(file);
    QString fileConfig(info.absolutePath()+QDir::separator()+info.baseName()+config_suffix);
    QFileInfo config(fileConfig);

    if(config.exists())
    {
        QFile file(fileConfig);

        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            const SweepClientSettings settings(file.readAll(), false);
            ptrUserInterface->onSweepClientSettings(settings);
            file.close();

            return true;
        }else
            return false;
    }

    return false;
}

void CoreSweepClient::launching()
{
    // connect to mqtt broker
    if(ptrMqttClient)
    {
        ptrMqttClient->setHostname(ptrUserInterface->sweepClientSettings().hostBroker());
        ptrMqttClient->setPort(ptrUserInterface->sweepClientSettings().portBroker());

        if (ptrMqttClient->state() == QMqttClient::Disconnected)
            ptrMqttClient->connectToHost();
    }
}

void CoreSweepClient::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    switch (ptrSweepTopic->sweepTopic(topic.name())) {
    case QSweepTopic::TOPIC_INFO:
    {
        QSweepAnswer answer(message);
        const QHackrfInfo info(answer.dataAnswer(), false);

        if(info.isValid()){
            ptrHackrfInfoModel->addResult(info);

//#ifdef QT_DEBUG
//            qDebug() << "---------------------------------------------------";
//            qDebug() << Q_FUNC_INFO << tr("Type Answer:") << static_cast<qint32>(answer.typeAnswer());
//            qDebug() << Q_FUNC_INFO << tr("Index Board:") << info.indexBoard();
//            qDebug() << Q_FUNC_INFO << tr("Serial Numbers:") << info.serialNumbers();
//            qDebug() << Q_FUNC_INFO << tr("Board ID Number:") << info.boardID();
//            qDebug() << Q_FUNC_INFO << tr("Firmware Version:") << info.firmwareVersion();
//            qDebug() << Q_FUNC_INFO << tr("Part ID Number:") << info.partIDNumber();
//            qDebug() << Q_FUNC_INFO << tr("Libhackrf Version:") << info.libHackrfVersion();
//            qDebug() << Q_FUNC_INFO << tr("Size message (byte):") << message.size();
//#endif
        }
    }
        break;
    case QSweepTopic::TOPIC_MESSAGE_LOG:
    {
        QSweepAnswer answer(message);
        QSweepMessageLog log(answer.dataAnswer());
        ptrMessageLogModel->addResult(log);
    }
        break;
    case QSweepTopic::TOPIC_POWER_SPECTR:
    {
        QSweepAnswer answer(message);
        QSweepSpectr powers(answer.dataAnswer());

        QVector<PowerSpectr> tmpPowerSpectr(powers.powerSpectr());

        std::sort(tmpPowerSpectr.begin(), tmpPowerSpectr.end(), [](const PowerSpectr& a, const PowerSpectr& b) {
            return a.m_frequency_min < b.m_frequency_min;
        });

        // for test
        ptrDataSource->updateDate(2400, 2500, tmpPowerSpectr);
        // update axis max & min
        ptrAxisY->setMin(ptrDataSource->minValue());
        ptrAxisY->setMax(ptrDataSource->maxValue());
        // test
        emit sendStartSpectr();

//#ifdef QT_DEBUG
//        qDebug() << "---------------------------------------------------";
//        qDebug() << tmpPowerSpectr.count() << tmpPowerSpectr.count()/4;

//        for(int i=0; i<tmpPowerSpectr.count(); ++i){
//            qDebug() << tmpPowerSpectr.at(i).m_frequency_min
//                     << tmpPowerSpectr.at(i).m_frequency_max
//                     << tmpPowerSpectr.at(i).m_fft_bin_width
//                     << tmpPowerSpectr.at(i).m_fft_size
//                     << tmpPowerSpectr.at(i).m_power;
//        }
//#endif
    }
        break;
    default:
        break;
    }

//#ifdef QT_DEBUG
//    qDebug() << "---------------------------------------------------";
//    qDebug() << Q_FUNC_INFO << topic.name() << ":" << message;
//    qDebug() << "---------------------------------------------------";
//#endif
}

void CoreSweepClient::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
            + QLatin1String(": State Change")
            + QString::number(ptrMqttClient->state());

    // Subscribers topic

    if (ptrMqttClient->state() == QMqttClient::Connected)
    {
        auto subscription = ptrMqttClient->subscribe(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_INFO));

        if (!subscription)
        {
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << "Could not subscribe. Is there a valid connection?";
#endif
            return;
        }

        auto subscription1 = ptrMqttClient->subscribe(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_MESSAGE_LOG));

        if (!subscription1)
        {
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << "Could not subscribe. Is there a valid connection?";
#endif
            return;
        }

        auto subscription2 = ptrMqttClient->subscribe(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_POWER_SPECTR));

        if (!subscription2)
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

void CoreSweepClient::brokerDisconnected()
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
}

void CoreSweepClient::pingReceived()
{
    ptrUserInterface->onPingReceived();
}

void CoreSweepClient::connecting()
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
}

void CoreSweepClient::sendingRequest(const QSweepRequest &value)
{
    if(ptrMqttClient) {
        if (ptrMqttClient->state() == QMqttClient::Connected) {
            qint32 result = ptrMqttClient->publish(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_CTRL), value.exportToJson());
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << tr("Data sending to host result:") << result << value.exportToJson();
#endif
        }
    }
}
