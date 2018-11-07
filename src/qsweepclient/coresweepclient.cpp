#include "coresweepclient.h"

#include <QApplication>
#include <QQmlContext>
#include <QFileInfo>
#include <QDir>
#include <QTime>
#include <QtCore/qdebug.h>

#include "userinterface.h"
#include "qsweeptopic.h"
#include "qsweeprequest.h"
#include "qsweepanswer.h"
#include "qsweepspectr.h"
#include "chart/datasource.h"
#include "chart/waterfallitem.h"
#include "systemmonitorinterface.h"
#include "statesweepclient.h"
#include "settings/sweepclientsettings.h"

#include "sweep_message.h"
#include "sdr_info.h"
#include "data_log.h"
#include "system_monitor.h"

static const QString config_suffix(QString(".conf"));

CoreSweepClient::CoreSweepClient(QObject *parent) : QObject(parent),
    ptrSweepTopic(new QSweepTopic(this)),
    ptrSystemMonitorInterface(new SystemMonitorInterface(this)),
    ptrStateSweepClient(new StateSweepClient(this))
{
    m_timerReceive = new QTime;
}

int CoreSweepClient::runCoreSweepClient(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("SweepClient");
    QCoreApplication::setApplicationVersion("1.0");

    QApplication app(argc, argv);

    initialization();

    if(readSettings(app.applicationFilePath()))
    {
        launching();
    }

    ptrEngine = new QQmlApplicationEngine(this);

    // Настройка осей графика
    ptrAxisX = new QValueAxis(this);
    ptrAxisX->setTitleText("Freq");
    ptrAxisX->setMin(ptrUserInterface->frequencyMin());
    ptrAxisX->setMax(ptrUserInterface->frequencyMax());
//    axisX->setLabelFormat("%i");
//    axisX->setTickCount(1);

    ptrAxisY = new QValueAxis(this);
    ptrAxisY->setTitleText("Level");
    ptrAxisY->setMin(ptrDataSource->minValueY());
    ptrAxisY->setMax(ptrDataSource->maxValueY());
//    axisY->setLabelFormat("%g");
//    axisY->setTickCount(5);

    qmlRegisterType<WaterfallItem>("waterfall", 1, 0, "Waterfall");

    QQmlContext *context = ptrEngine->rootContext();
    context->setContextProperty("userInterface", ptrUserInterface);
    context->setContextProperty("hackrfInfoModel", ptrHackrfInfoModel);
    context->setContextProperty("messageLogModel", ptrMessageLogModel);
    context->setContextProperty("dataSource", ptrDataSource);
    context->setContextProperty("valueAxisY", ptrAxisY);
    context->setContextProperty("valueAxisX", ptrAxisX);
    context->setContextProperty("systemMonitorInterface", ptrSystemMonitorInterface);
    context->setContextProperty("stateSweepClient", ptrStateSweepClient);

    ptrEngine->load(QUrl(QLatin1String("qrc:/main.qml")));

    QObject *rootObject = ptrEngine->rootObjects().first();
//    QObject *qmlChartView = rootObject->findChild<QObject*>("chartViewSpectr");
//    qmlChartView->setProperty("title", tr("Spectr"));

    QObject *qmlPlotWaterfall = rootObject->findChild<QObject*>("plotWaterfall");
    WaterfallItem *plotWaterfall = static_cast<WaterfallItem *>(qmlPlotWaterfall);

    connect(ptrDataSource, &DataSource::sendPowerSpectr,
            plotWaterfall, &WaterfallItem::onPowerSpectr);

    if (ptrEngine->rootObjects().isEmpty())
        return -1;

    return app.exec();
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
    ptrMqttClient = new QMqttClient(this);
    connect(ptrMqttClient, &QMqttClient::messageReceived,
            this, &CoreSweepClient::messageReceived);

    connect(ptrMqttClient, &QMqttClient::messageReceived,
            this, &CoreSweepClient::slot_message_received);


    connect(ptrMqttClient, &QMqttClient::stateChanged,
            this, &CoreSweepClient::updateLogStateChange);
    connect(ptrMqttClient, &QMqttClient::disconnected,
            this, &CoreSweepClient::brokerDisconnected);
    connect(ptrMqttClient, &QMqttClient::connected,
            this, &CoreSweepClient::pingReceived);
    connect(ptrMqttClient, &QMqttClient::pingResponseReceived,
            this , &CoreSweepClient::pingReceived);
    connect(ptrMqttClient, &QMqttClient::errorChanged,
            this, &CoreSweepClient::errorChanged);

    // System Monitor Interface
    connect(this, &CoreSweepClient::signal_system_monitor,
            ptrSystemMonitorInterface, &SystemMonitorInterface::slot_system_monitor);

    // DataSource
    ptrDataSource = new DataSource(this);

    // user interface
    ptrUserInterface = new UserInterface(this);
    // new style
    connect(ptrUserInterface, &UserInterface::signal_sweep_message,
            this, &CoreSweepClient::slot_publish_message);

    connect(this, &CoreSweepClient::sendStartSpectr,
            ptrUserInterface, &UserInterface::sendStartSpectr);
    connect(ptrUserInterface, &UserInterface::sendClearMaxPowerSpectr,
            ptrDataSource, &DataSource::clearMaxPowerSpectr);
    // StateSweepClient
    connect(this, &CoreSweepClient::sendStateConnected,
            ptrStateSweepClient, &StateSweepClient::onConnect);
    connect(this, &CoreSweepClient::sendStateConnected,
            ptrUserInterface, &UserInterface::onRequestSweepInfo);
    connect(this, &CoreSweepClient::sendStateDisconnected,
            ptrStateSweepClient, &StateSweepClient::onDisconnect);

    // Hackrf Info Model
    ptrHackrfInfoModel = new HackrfInfoModel(this);
    connect(this, &CoreSweepClient::signal_sdr_info,
            ptrHackrfInfoModel, &HackrfInfoModel::add_result);

    // Message Log Model
    ptrMessageLogModel = new MessageLogModel(this);
    connect(this, &CoreSweepClient::signal_data_log,
            ptrMessageLogModel, &MessageLogModel::add_result);
}

bool CoreSweepClient::readSettings(const QString &file)
{
    bool isRead(false);

    if(!file.isEmpty())
    {
        QFileInfo info(file);
        const QString fileConfig(info.absolutePath()+QDir::separator()+info.baseName()+config_suffix);
        QFileInfo config(fileConfig);

        if(config.exists())
        {
            QFile file(fileConfig);

            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                const SweepClientSettings settings(file.readAll(), false); // !!!!!!!!!!!!!!!
                ptrSweepClientSettings = new SweepClientSettings(settings);
                file.close();

                isRead = ptrSweepClientSettings->isValid();

                ptrUserInterface->onSweepClientSettings(settings);
                // MessageLogModel
                ptrMessageLogModel->setMaxSize(ptrSweepClientSettings->maxSizeMessageLog());


                return isRead;
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

bool CoreSweepClient::saveSettings(const QString &file)
{
    bool isSave(false);

    if(!file.isEmpty())
    {
        const QFileInfo info(file);
        const QString fileConfig(info.absolutePath()+QDir::separator()+info.baseName()+config_suffix);
        QFileInfo config(fileConfig);
        bool fileExists = config.exists();

        QFile file(fileConfig);

        if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            if(fileExists){
                if(ptrSweepClientSettings)
                    file.write(ptrSweepClientSettings->exportToJson());
            }else{
                const auto defaultSettings = SweepClientSettings();
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

    // start timer
    m_timerReceive->start();
    m_sizeDatacReceive = 0;
}

void CoreSweepClient::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    switch (ptrSweepTopic->sweepTopic(topic.name()))
    {
    case QSweepTopic::TOPIC_POWER_SPECTR:
    {
        QSweepAnswer answer(message);
        QSweepSpectr powers(answer.dataAnswer());

        QVector<PowerSpectr> tmpPowerSpectr(powers.powerSpectr());

        std::sort(tmpPowerSpectr.begin(), tmpPowerSpectr.end(), [](const PowerSpectr& a, const PowerSpectr& b) {
            return a.m_frequency_min < b.m_frequency_min;
        });

        // for test
        ptrDataSource->updateDate(ptrUserInterface->frequencyMin(),
                                  ptrUserInterface->frequencyMax(),
                                  tmpPowerSpectr);

        // X update axis max & min freq
        ptrAxisX->setMin(ptrUserInterface->frequencyMin());
        ptrAxisX->setMax(ptrUserInterface->frequencyMax());
        // test
        emit sendStartSpectr();
    }
        break;

    default:
        break;
    }

#ifdef QT_DEBUG
//    qDebug() << "---------------------------------------------------";
//    qDebug() << Q_FUNC_INFO << topic.name() << ":" << "message size:" << message.size() << "byte";
//    qDebug() << "---------------------------------------------------";
#endif

}

void CoreSweepClient::updateLogStateChange()
{
#ifdef QT_DEBUG
    const QString content = QDateTime::currentDateTime().toString()
            + QLatin1String(": State Change ")
            + QString::number(ptrMqttClient->state());

    qDebug() << Q_FUNC_INFO << content;
#endif

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

        auto subscription3 = ptrMqttClient->subscribe(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_SYSTEM_MONITOR));

        if (!subscription3)
        {
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << "Could not subscribe. Is there a valid connection?";
#endif
            return;
        }

        emit sendStateConnected();
    }

    if (ptrMqttClient->state() == QMqttClient::Disconnected)
    {
        emit sendStateDisconnected();
    }
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

void CoreSweepClient::slot_publish_message(const QByteArray &value)
{
    if(ptrMqttClient)
    {
        if (ptrMqttClient->state() == QMqttClient::Connected)
        {
            const sweep_message send_data(value);

            if(send_data.is_valid())
            {
                if(send_data.type() == type_message::CTRL_INFO)
                    ptrMqttClient->publish(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_CTRL), send_data.export_json());

                if(send_data.type() == type_message::CTRL_SPECTR)
                    ptrMqttClient->publish(ptrSweepTopic->sweepTopic(QSweepTopic::TOPIC_CTRL), send_data.export_json());
            }

            //#ifdef QT_DEBUG
            //            qDebug() << Q_FUNC_INFO << tr("Data sending to host result:") << result << value.export_json();
            //#endif
        }
    }
}

void CoreSweepClient::slot_message_received(const QByteArray &message, const QMqttTopicName &topic)
{
    // system info
    if(ptrSweepTopic->sweepTopic(topic.name()) == QSweepTopic::TOPIC_INFO)
    {
        const sweep_message data_received(message, false);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::DATA_INFO)
            {
                const sdr_info sdr_info_data(data_received.data_message(), false);
                emit signal_sdr_info(sdr_info_data);
            }
        }
    }

    // message log
    if(ptrSweepTopic->sweepTopic(topic.name()) == QSweepTopic::TOPIC_MESSAGE_LOG)
    {
        const sweep_message data_received(message, false);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::DATA_MESSAGE_LOG)
            {
                const data_log data_log_data(data_received.data_message(), false);
                emit signal_data_log(data_log_data);
            }
        }
    }

    // system monitor
    if(ptrSweepTopic->sweepTopic(topic.name()) == QSweepTopic::TOPIC_SYSTEM_MONITOR)
    {
        const sweep_message data_received(message, false);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::DATA_SYSTEM_MONITOR)
            {
                const system_monitor data_system_monitor(data_received.data_message(), false);
                emit signal_system_monitor(data_system_monitor);
            }
        }
    }

    m_sizeDatacReceive = m_sizeDatacReceive + message.size();

    if(m_timerReceive->elapsed()>=5000)
    {
#ifdef QT_DEBUG
        qDebug() << Q_FUNC_INFO
                 << "Data size:" << QString::number(m_sizeDatacReceive/1024.0, 'f', 2) << "Kbyte"
                 << "(" << QString::number((m_sizeDatacReceive/1024)/1024.0, 'f', 2) << "Mbyte" << ")"
                 << QString("Time elapsed: %1 ms, %2 s").arg(m_timerReceive->elapsed()).arg(m_timerReceive->elapsed()/1000);
#endif
        m_timerReceive->restart();
        m_sizeDatacReceive = 0;
    }
}

void CoreSweepClient::errorChanged(QMqttClient::ClientError error)
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
