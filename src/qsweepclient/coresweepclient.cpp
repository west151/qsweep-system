#include "coresweepclient.h"

#include <QApplication>
#include <QQmlContext>
#include <QFileInfo>
#include <QDir>
#include <QTime>
#include <QtCore/qdebug.h>
#include <QThread>
#include <QStandardPaths>

#include "userinterface.h"
#include "sweep_topic.h"
#include "chart/surface_spectr.h"
#include "systemmonitorinterface.h"
#include "statesweepclient.h"
#include "settings/sweepclientsettings.h"
#include "template/ranges_template.h"

#include "sweep_message.h"
#include "sdr_info.h"
#include "data_log.h"
#include "system_monitor.h"
#include "data_spectr.h"
#include "spectr/ta_spectr.h"
#include "database/db_local_state.h"

static const QString str_config_suffix(QString(".conf"));
static const QString str_path_template(QString("template"));
static const QString str_template_suffix(QString(".template"));

CoreSweepClient::CoreSweepClient(QObject *parent) : QObject(parent),
    ptr_sweep_topic(new sweep_topic(this)),
    ptrSystemMonitorInterface(new SystemMonitorInterface(this)),
    ptrStateSweepClient(new StateSweepClient(this))
{
    m_timer_receive = new QTime;

    qRegisterMetaType<data_spectr>();
    qRegisterMetaType<QVector<params_spectr> >();
    qRegisterMetaType<ranges_template>();
}

int CoreSweepClient::runCoreSweepClient(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("sdr_qsweep");
    QCoreApplication::setApplicationName("qsweep-cli");
    QCoreApplication::setApplicationVersion("1.0");

    QApplication app(argc, argv);

    initialization();

    QString file_path = app.applicationFilePath();

    if(read_settings(file_path))
    {
        launching();
        read_template(file_path);   // template
    }

    ptrEngine = new QQmlApplicationEngine(this);

    qmlRegisterType<surface_spectr>("surfacespectr", 1, 0, "SurfaceSpectr");

    QQmlContext *context = ptrEngine->rootContext();
    context->setContextProperty("userInterface", ptrUserInterface);
    context->setContextProperty("hackrfInfoModel", ptr_hackrf_info_model);
    context->setContextProperty("messageLogModel", ptr_message_log_model);
    context->setContextProperty("systemMonitorInterface", ptrSystemMonitorInterface);
    context->setContextProperty("stateSweepClient", ptrStateSweepClient);
    context->setContextProperty("paramsSpectrModel", ptr_params_spectr_model);
    context->setContextProperty("rangesTemplateModel", ptr_ranges_template_model);

    ptrEngine->load(QUrl(QLatin1String("qrc:/main.qml")));

    QObject *rootObject = ptrEngine->rootObjects().first();

    // Spect surface
    QObject *qmlSurfaceSpectr = rootObject->findChild<QObject*>("objSurfaceSpectr");
    surface_spectr *surfaceSpectr = static_cast<surface_spectr *>(qmlSurfaceSpectr);

    connect(ptrUserInterface, &UserInterface::signal_spectr_max_calc,
            surfaceSpectr, &surface_spectr::slot_spectr_max_calc);

    // ta spectr
    ptr_ta_spectr_worker = new ta_spectr;
    ptr_ta_spectr_thread = new QThread;
    ptr_ta_spectr_worker->moveToThread(ptr_ta_spectr_thread);

    connect(this, &CoreSweepClient::signal_data_spectr,
            ptr_ta_spectr_worker, &ta_spectr::slot_data_spectr);

    connect(ptr_ta_spectr_worker, &ta_spectr::signal_spectr_rt,
            surfaceSpectr, &surface_spectr::slot_power_spectr);

    ptr_ta_spectr_thread->start();

    if (ptrEngine->rootObjects().isEmpty())
        return -1;

    return app.exec();
}

void CoreSweepClient::onDisconnectFromHost()
{
    if (ptrMqttClient->state() == QMqttClient::Connected){
        //ptrMqttClient->unsubscribe(ptrSweepTopic->sweep_topic_by_type(sweep_topic::TOPIC_DATA));
        ptrMqttClient->disconnectFromHost();
    }
}

void CoreSweepClient::initialization()
{
    // MQTT
    ptrMqttClient = new QMqttClient(this);
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

    // user interface
    ptrUserInterface = new UserInterface(this);
    connect(ptrUserInterface, &UserInterface::signal_sweep_message,
            this, &CoreSweepClient::slot_publish_message);

    // StateSweepClient
    connect(this, &CoreSweepClient::sendStateConnected,
            ptrStateSweepClient, &StateSweepClient::onConnect);
    connect(this, &CoreSweepClient::sendStateConnected,
            ptrUserInterface, &UserInterface::onRequestSweepInfo);
    connect(this, &CoreSweepClient::sendStateDisconnected,
            ptrStateSweepClient, &StateSweepClient::onDisconnect);

    // Hackrf Info Model
    ptr_hackrf_info_model = new hackrf_info_model(this);
    connect(this, &CoreSweepClient::signal_sdr_info,
            ptr_hackrf_info_model, &hackrf_info_model::add_result);

    // Message Log Model
    ptr_message_log_model = new message_log_model(this);
    connect(this, &CoreSweepClient::signal_data_log,
            ptr_message_log_model, &message_log_model::add_result);

    // database local
    ptr_db_local_state_worker = new db_local_state;
    ptr_db_local_thread = new QThread;
    ptr_db_local_state_worker->moveToThread(ptr_db_local_thread);

    connect(this, &CoreSweepClient::signal_open_db,
            ptr_db_local_state_worker, &db_local_state::slot_open_db);
    connect(ptrUserInterface, &UserInterface::signal_read_params_spectr,
            ptr_db_local_state_worker, &db_local_state::slot_read_params_spectr);
    // save params
    connect(ptrUserInterface, &UserInterface::signal_save_params_spectr,
            ptr_db_local_state_worker, &db_local_state::slot_write_params_spectr);

    // params spectr model
    ptr_params_spectr_model = new params_spectr_model(this);
    connect(ptr_db_local_state_worker, &db_local_state::signal_read_params_spectr,
            ptr_params_spectr_model, &params_spectr_model::slot_set_vector_result);

    // send params from model
    connect(ptr_params_spectr_model, &params_spectr_model::signal_params_from_model,
            ptrUserInterface, &UserInterface::slot_set_params_spectr);

    // sync db & model
    connect(ptr_params_spectr_model, &params_spectr_model::signal_remove_from_model,
            ptr_db_local_state_worker, &db_local_state::slot_remove_params_spectr);

    ptr_db_local_thread->start();

    // ranges template model
    ptr_ranges_template_model = new ranges_template_model(this);

    connect(this, &CoreSweepClient::signal_ranges_template,
            ptr_ranges_template_model, &ranges_template_model::add_result);
}

bool CoreSweepClient::read_settings(const QString &file)
{
    bool isRead(false);

    if(!file.isEmpty())
    {
        QFileInfo info(file);
        const QString fileConfig(info.absolutePath()+QDir::separator()+info.baseName()+str_config_suffix);
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
                ptr_message_log_model->set_max_size(ptrSweepClientSettings->maxSizeMessageLog());


                return isRead;
            }else{
                qCritical("Can't file open ('%s').", qUtf8Printable(config.fileName()));
                qCritical("Error: '%s'", qUtf8Printable(file.errorString()));
            }
        }else{
            qCritical("File '%s' does not exist!", qUtf8Printable(config.fileName()));

            fprintf(stderr, "Create new file settings: %s\n", qUtf8Printable(fileConfig));
            save_settings(fileConfig);
        }
    }

    return isRead;
}

bool CoreSweepClient::save_settings(const QString &file)
{
    bool isSave(false);

    if(!file.isEmpty())
    {
        const QFileInfo info(file);
        const QString fileConfig(info.absolutePath()+QDir::separator()+info.baseName()+str_config_suffix);
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

bool CoreSweepClient::read_template(const QString &path)
{
    bool result(false);

    if(!path.isEmpty())
    {
        const QFileInfo info(path);
        const QDir directory(info.absolutePath() + QDir::separator() + str_path_template);
        QStringList file_list = directory.entryList(QStringList() << "*" + str_template_suffix, QDir::Files);

        for(int i=0; i<file_list.size(); ++i)
        {
            QString file = info.absolutePath()
                    + QDir::separator()
                    + str_path_template
                    + QDir::separator()
                    + file_list.at(i);

            QFile file_renge(file);

            if(file_renge.open(QIODevice::ReadOnly | QIODevice::Text))
            {
#ifdef QT_DEBUG
                qDebug() << Q_FUNC_INFO << file;
#endif
                const ranges_template range(file_renge.readAll(), false);
                file_renge.close();

                if(range.is_valid())
                    emit signal_ranges_template(range);

            }else{
                qCritical("Can't file open ('%s').", qUtf8Printable(file_renge.fileName()));
                qCritical("Error: '%s'", qUtf8Printable(file_renge.errorString()));
            }
        }
    }
    return result;
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
    m_timer_receive->start();
    m_size_data_receive = 0;

    // open/create database
    emit signal_open_db(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
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
        auto subscription = ptrMqttClient->subscribe(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_info));

        if (!subscription)
        {
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << "Could not subscribe. Is there a valid connection?";
#endif
            return;
        }

        auto subscription1 = ptrMqttClient->subscribe(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_message_log));

        if (!subscription1)
        {
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << "Could not subscribe. Is there a valid connection?";
#endif
            return;
        }

        auto subscription2 = ptrMqttClient->subscribe(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_power_spectr));

        if (!subscription2)
        {
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << "Could not subscribe. Is there a valid connection?";
#endif
            return;
        }

        auto subscription3 = ptrMqttClient->subscribe(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_system_monitor));

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

void CoreSweepClient::slot_publish_message(const QByteArray &value)
{
    if(ptrMqttClient)
    {
        if (ptrMqttClient->state() == QMqttClient::Connected)
        {
            const sweep_message send_data(value);

            if(send_data.is_valid())
            {
                if(send_data.type() == type_message::ctrl_info)
                    ptrMqttClient->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_ctrl), send_data.export_json());

                if(send_data.type() == type_message::ctrl_spectr)
                    ptrMqttClient->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_ctrl), send_data.export_json());

                if(send_data.type() == type_message::ctrl_db)
                    ptrMqttClient->publish(ptr_sweep_topic->sweep_topic_by_type(sweep_topic::topic_db_ctrl), send_data.export_json());
            }

//#ifdef QT_DEBUG
//            qDebug() << Q_FUNC_INFO << tr("Data sending to host result:") << send_data.export_json();
//#endif
        }
    }
}

void CoreSweepClient::slot_message_received(const QByteArray &message, const QMqttTopicName &topic)
{
    // system info
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_info)
    {
        const sweep_message data_received(message, false);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::data_sdr_info)
            {
                const sdr_info sdr_info_data(data_received.data_message(), false);
                emit signal_sdr_info(sdr_info_data);
            }
        }
    }

    // message log
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_message_log)
    {
        const sweep_message data_received(message, false);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::data_message_log)
            {
                const data_log data_log_data(data_received.data_message(), false);
                emit signal_data_log(data_log_data);
            }
        }
    }

    // system monitor
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_system_monitor)
    {
        const sweep_message data_received(message, false);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::data_system_monitor)
            {
                const system_monitor data_system_monitor(data_received.data_message(), false);
                emit signal_system_monitor(data_system_monitor);
            }
        }
    }

    // power spectr
    if(ptr_sweep_topic->sweep_topic_by_str(topic.name()) == sweep_topic::topic_power_spectr)
    {
        const sweep_message data_received(message, false);

        if(data_received.is_valid())
        {
            if(data_received.type() == type_message::data_spectr)
            {
                const data_spectr powers(data_received.data_message());

                emit signal_data_spectr(powers);
            }
        }
    }

    m_size_data_receive = m_size_data_receive + message.size();

    if(m_timer_receive->elapsed()>=5000)
    {
#ifdef QT_DEBUG
        qDebug() << Q_FUNC_INFO
                 << "Data size:" << QString::number(m_size_data_receive/1024.0, 'f', 2) << "Kbyte"
                 << "(" << QString::number((m_size_data_receive/1024)/1024.0, 'f', 2) << "Mbyte" << ")"
                 << QString("Time elapsed: %1 ms, %2 s").arg(m_timer_receive->elapsed()).arg(m_timer_receive->elapsed()/1000);
#endif
        m_timer_receive->restart();
        m_size_data_receive = 0;
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
    default:
        qCritical("Unknown error.");
        break;
    }
}
