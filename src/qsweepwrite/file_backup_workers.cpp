#include "file_backup_workers.h"

#include <QDir>
#include <QDateTime>

#include "database/db_const.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

file_backup_workers::file_backup_workers(QObject *parent) : QObject(parent)
{
    setObjectName(this->metaObject()->className());

    is_ready = false;
}

void file_backup_workers::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void file_backup_workers::slot_initialization()
{
    QDir dir(m_settings.backup_path());

    if (!dir.exists())
        dir.mkpath(".");

    if(dir.exists())
    {
        is_ready = true;

        emit signal_update_state_workers(state_workers::initialization);

    }else {
#ifdef QT_DEBUG
        qCritical() << "Can't find path:" << dir.absolutePath();
#endif
    }

}

void file_backup_workers::slot_launching()
{
    emit signal_update_state_workers(state_workers::launching);
}

void file_backup_workers::slot_stopping()
{
    emit signal_update_state_workers(state_workers::stopping);
}

void file_backup_workers::slot_file_backup(const QString &file_name)
{
    if(is_ready)
    {
        QFileInfo in_file_info(file_name);
        QString file_name_gen = QDateTime::currentDateTimeUtc().toString("ddMMyyyy_hhmmss");
        file_name_gen.append(".backup");
        QFileInfo out_file_info(m_settings.backup_path()+QDir::separator()+file_name_gen);

        if(in_file_info.exists())
        {

#ifdef QT_DEBUG
            qDebug() << "Input file name:" << in_file_info.filePath();
            qDebug() << "Output file name:" << out_file_info.filePath();
#endif
            file_compress(in_file_info.filePath(), out_file_info.filePath(), m_settings.backup_compress_level());

#ifdef QT_DEBUG
            qDebug() << "File compressed:" << out_file_info.filePath();
#endif
            file_uncompressed(out_file_info.filePath(), m_settings.backup_path()+QDir::separator()+"uncompress.sqlite");

#ifdef QT_DEBUG
            qDebug() << "File uncompressed:" << out_file_info.filePath();
#endif

        }
    }
}
