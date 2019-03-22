#include "core_sweep_write.h"

#include <QFileInfo>
#include <QDir>

#include "sweep_write_settings.h"

static const QString config_suffix(QString(".conf"));

core_sweep_write::core_sweep_write(const QString &file, QObject *parent) : QObject(parent)
{
    // read file settings
    const auto is_settings = read_settings(file);

    if(is_settings)
    {
        initialization();
    } else {
        qCritical("Error: Read settings.");
    }
}

void core_sweep_write::initialization()
{

}

void core_sweep_write::launching()
{

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
                if(ptr_sweep_write_settings)
                    file.write(ptr_sweep_write_settings->exportToJson());
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
                ptr_sweep_write_settings = new sweep_write_settings(file.readAll(), false);
                file.close();

                is_read = ptr_sweep_write_settings->is_valid();

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
