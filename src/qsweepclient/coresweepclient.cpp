#include "coresweepclient.h"

#include <QGuiApplication>
#include <QQmlContext>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>

#include "userinterface.h"
#include "qsweeptopic.h"
#include "qsweeprequest.h"
#include "qsweepanswer.h"
#include "qhackrfinfo.h"
#include "qsweepmessagelog.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

static const QString config_suffix(QString(".json"));

CoreSweepClient::CoreSweepClient(QObject *parent) : QObject(parent),
    ptrUserInterface(new UserInterface(this)),
    ptrMqttClient(new QMqttClient(this)),
    ptrSweepTopic(new QSweepTopic(this)),
    ptrHackrfInfoModel(new HackrfInfoModel(this)),
    ptrMessageLogModel(new MessageLogModel(this))
{
//    https://stackoverflow.com/questions/49560780/mqt-qmqtt-qt-core-module-in-thread-strange-behaviour-from-animal-help-project
}

int CoreSweepClient::runCoreSweepClient(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("CoreSweepClient");
    QCoreApplication::setApplicationVersion("1.0");

    QGuiApplication app(argc, argv);

    initialization();

    if(readSettings(app.applicationFilePath())){
        launching();
    }

    ptrEngine = new QQmlApplicationEngine(this);

    QQmlContext *context = ptrEngine->rootContext();
    context->setContextProperty("userInterface", ptrUserInterface);
    context->setContextProperty("hackrfInfoModel", ptrHackrfInfoModel);
    context->setContextProperty("messageLogModel", ptrMessageLogModel);
    ptrEngine->load(QUrl(QLatin1String("qrc:/main.qml")));

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
    // User interface
    connect(ptrUserInterface, &UserInterface::sendConnectToHost,
            this, &CoreSweepClient::onConnectToHost);
    connect(ptrUserInterface, &UserInterface::sendDisconnectFromHost,
            this, &CoreSweepClient::onDisconnectFromHost);

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
            SweepClientSettings settings(file.readAll());
            ptrUserInterface->onSweepClientSettings(settings);
            file.close();
        }else
            return false;
    }

    return false;
}

bool CoreSweepClient::launching() const
{

    return false;
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

//#ifdef QT_DEBUG
//        qDebug() << "---------------------------------------------------";
//        qDebug() << Q_FUNC_INFO << tr("DateTime:") << log.dateTime();
//        qDebug() << Q_FUNC_INFO << tr("Message Log:") << log.textMessage();
//#endif
    }
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
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
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
