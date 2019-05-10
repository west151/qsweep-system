#include "file_backup_workers.h"

#include <QDir>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

file_backup_workers::file_backup_workers(QObject *parent) : QObject(parent)
{

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
