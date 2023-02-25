#include "db_reader_worker.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_reader_worker::db_reader_worker(QObject *parent) : QObject(parent)
{
    setObjectName(this->metaObject()->className());
}

void db_reader_worker::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void db_reader_worker::slot_initialization()
{
    // to do

    emit signal_update_state_workers(state_workers::initialization);
}

void db_reader_worker::slot_launching()
{
    // to do

    emit signal_update_state_workers(state_workers::launching);
}

void db_reader_worker::slot_stopping()
{
    // to do

    emit signal_update_state_workers(state_workers::stopping);
}
