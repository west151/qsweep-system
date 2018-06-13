#include "coresweepclient.h"

#include <QGuiApplication>
#include <QQmlContext>
#include <QDateTime>

#include "userinterface.h"
#include "qsweeptopic.h"
#include "qsweeprequest.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

CoreSweepClient::CoreSweepClient(QObject *parent) : QObject(parent),
    ptrUserInterface(new UserInterface(this)),
    ptrMqttClient(new QMqttClient(this)),
    ptrSweepTopic(new QSweepTopic(this))
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

    ptrEngine = new QQmlApplicationEngine(this);

    ptrEngine->load(QUrl(QLatin1String("qrc:/main.qml")));
    QQmlContext *context = ptrEngine->rootContext();
    context->setContextProperty("userInterface", ptrUserInterface);

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
    connect(ptrUserInterface, &UserInterface::sendMessageToHost,
            this, &CoreSweepClient::sendingMessage);
    connect(ptrUserInterface, &UserInterface::sendRequestSweepInfo,
            this, &CoreSweepClient::sendingRequest);
}

void CoreSweepClient::launching()
{

}

void CoreSweepClient::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO << topic.name() << ":" << message;
#endif
}

void CoreSweepClient::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
            + QLatin1String(": State Change")
            + QString::number(ptrMqttClient->state());

    if (ptrMqttClient->state() == QMqttClient::Connected)
    {
        auto subscription = ptrMqttClient->subscribe(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_DATA));

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

void CoreSweepClient::sendingMessage()
{
    if(ptrMqttClient) {
        if (ptrMqttClient->state() == QMqttClient::Connected) {
            qint32 result = ptrMqttClient->publish(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_CTRL), "run");
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << tr("Data sending to host result:") << result << "test";
#endif
        }
    }
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
