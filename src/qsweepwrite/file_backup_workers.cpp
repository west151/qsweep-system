#include "file_backup_workers.h"

file_backup_workers::file_backup_workers(QObject *parent) : QObject(parent)
{

}

void file_backup_workers::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void file_backup_workers::slot_initialization()
{
    emit signal_update_state_workers(state_workers::initialization);
}

void file_backup_workers::slot_launching()
{
    emit signal_update_state_workers(state_workers::launching);
}

void file_backup_workers::slot_stopping()
{
    emit signal_update_state_workers(state_workers::stopping);
}
