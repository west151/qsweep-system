#include "core_sweep_write.h"

#include <QFileInfo>
#include <QDir>

#include "provider/mqtt_provider.h"
#include "database/db_manager.h"

static const QString config_suffix(QString(".conf"));

core_sweep_write::core_sweep_write(const QString &file, QObject *parent) : QObject(parent)
{
    // read file settings
    const auto is_settings = read_settings(file);

    if(is_settings)
    {
        initialization();
        launching();
    } else {
        qCritical("Error: Read settings.");
    }
}

void core_sweep_write::initialization()
{
    // mqtt provider
    ptr_mqtt_provider = new mqtt_provider(this);
    ptr_mqtt_provider->set_configuration(m_sweep_write_settings);
    ptr_mqtt_provider->initialization();    

    // database manager
    ptr_db_manager = new db_manager(this);
    ptr_db_manager->set_configuration(m_sweep_write_settings);
    ptr_db_manager->initialization();

    // connect signals and slots
}

void core_sweep_write::launching()
{
    ptr_mqtt_provider->launching();
}

bool core_sweep_write::save_settings(const QString &file)
{
    bool is_save(false);

    if(!file.isEmpty())
    {
        QFileInfo info(file);
        QString fileConfig(info.absolutePath()+QDir::separator()+info.baseName()+config_suffix);
        QFileInfo config(fileConfig);
        bool fileExists = config.exists();

        QFile file(fileConfig);

        if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            if(fileExists){
                if(m_sweep_write_settings.is_valid())
                    file.write(m_sweep_write_settings.exportToJson());
            }else{
                const auto defaultSettings = sweep_write_settings();
                file.write(defaultSettings.exportToJson(false, false));
            }

            file.close();
            is_save = true;
        }else{
            qCritical("Can't file open ('%s').", qUtf8Printable(config.fileName()));
            qCritical("Error: '%s'", qUtf8Printable(file.errorString()));
        }
    }

    return is_save;
}

bool core_sweep_write::read_settings(const QString &file)
{
    bool is_read(false);

    if(!file.isEmpty())
    {
        QFileInfo info(file);
        QString fileConfig(info.absolutePath()+QDir::separator()+info.baseName()+config_suffix);
        QFileInfo config(fileConfig);

        if(config.exists())
        {
            QFile file(fileConfig);

            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                const auto settings = sweep_write_settings(file.readAll(), false);
                m_sweep_write_settings = settings;
                file.close();

                is_read = m_sweep_write_settings.is_valid();

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

    return is_read;
}
