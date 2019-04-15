#include "db_path_monitor.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_path_monitor::db_path_monitor(QObject *parent) : QObject(parent)
{
    setObjectName(this->metaObject()->className());
}

void db_path_monitor::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void db_path_monitor::slot_initialization()
{
    // to do

    emit signal_update_state_workers(state_workers_type::initialization);
}

void db_path_monitor::slot_launching()
{
    // to do

    emit signal_update_state_workers(state_workers_type::launching);
}

void db_path_monitor::slot_stopping()
{
    // to do

    emit signal_update_state_workers(state_workers_type::stopping);
}

