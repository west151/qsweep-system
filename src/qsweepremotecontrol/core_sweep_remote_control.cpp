#include "core_sweep_remote_control.h"

#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QQmlContext>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "model/top_model.h"

static const QString str_config_suffix(QString(".conf"));

core_sweep_remote_control::core_sweep_remote_control(const QString &app_name, QObject *parent) : QObject(parent),
    ptr_top_model(new top_model(this))
{
    str_config_location.append(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    str_config_location.append(QDir::separator());
    str_config_location.append(app_name);
    str_config_location.append(str_config_suffix);

    ptr_remote_control_settings = new remote_control_settings;
}

bool core_sweep_remote_control::initialization()
{
    bool status(false);

    qInfo("Сonfiguration file: '%s'", qUtf8Printable(str_config_location));

    if(QFile::exists(str_config_location))
        status = read_settings();
    else
        status = write_settings();

    return status;
}

void core_sweep_remote_control::program_launch(bool is_init_state)
{
    ptr_engine = new QQmlApplicationEngine(this);

    QQmlContext *context = ptr_engine->rootContext();
    context->setContextProperty("top_model", ptr_top_model);

    ptr_engine->load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    ptr_top_model->start();
}

bool core_sweep_remote_control::read_settings()
{
    bool is_read(false);

    if(!str_config_location.isEmpty())
    {
        QFile config_file(str_config_location);

        if(config_file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            if(ptr_remote_control_settings)
            {
                const auto settings = remote_control_settings(config_file.readAll(), false);
                ptr_remote_control_settings->set_settings(settings);

                if(ptr_remote_control_settings->is_valid())
                    is_read = true;
            }
            config_file.close();
        } else {
            qCritical("Can't file open ('%s').", qUtf8Printable(config_file.fileName()));
            qCritical("Error: '%s'", qUtf8Printable(config_file.errorString()));
        }
    }

    return is_read;
}

bool core_sweep_remote_control::write_settings()
{
    bool is_write(false);

    if(!str_config_location.isEmpty())
    {
        QFile config_file(str_config_location);

        if(config_file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            if(ptr_remote_control_settings){
                qint64 result = config_file.write(ptr_remote_control_settings->to_json(false, false));

                if(result > 0)
                    is_write = true;
            }
            config_file.close();
        } else {
            qCritical("Can't file open ('%s').", qUtf8Printable(config_file.fileName()));
            qCritical("Error: '%s'", qUtf8Printable(config_file.errorString()));
        }
    }

    return is_write;
}
