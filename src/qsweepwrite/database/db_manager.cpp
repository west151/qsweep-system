#include "db_manager.h"

#include <QThread>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_manager::db_manager(QObject *parent) : QObject(parent)
{
    ptr_db_state_workers = new db_state_workers(this);

    // is all initialization
    connect(ptr_db_state_workers, &db_state_workers::signal_all_initialization,
            this, &db_manager::slot_is_all_initialization_workers);
    // is all launching
    connect(ptr_db_state_workers, &db_state_workers::signal_all_launching,
            this, &db_manager::slot_is_all_launching_workers);
    // is all stopping
    connect(ptr_db_state_workers, &db_state_workers::signal_all_stopping,
            this, &db_manager::slot_is_all_stopping_workers);
}

void db_manager::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void db_manager::initialization()
{
    create_db_system_worker(ptr_db_state_workers);

    create_db_writer_worker(ptr_db_state_workers);

    create_db_path_monitor_worker(ptr_db_state_workers);

    emit signal_initialization_workers();
}

void db_manager::launching()
{

}

void db_manager::stopping()
{

}

void db_manager::slot_is_all_initialization_workers()
{

}

void db_manager::slot_is_all_launching_workers()
{

}

void db_manager::slot_is_all_stopping_workers()
{

}

void db_manager::slot_received_data(const QByteArray &rc_data)
{

}

void db_manager::create_db_system_worker(db_state_workers *state)
{
    ptr_db_system_worker = new db_system_worker;
    ptr_db_system_worker->set_configuration(m_settings);

    // add "db_system_worker" to state monitor
    state->add_name_workers(ptr_db_system_worker->metaObject()->className());

    ptr_db_system_thread = new QThread;
    ptr_db_system_worker->moveToThread(ptr_db_system_thread);

    // initialization
    connect(this, &db_manager::signal_initialization_workers,
            ptr_db_system_worker, &db_system_worker::slot_initialization);
    // launching
    connect(this, &db_manager::signal_launching_workers,
            ptr_db_system_worker, &db_system_worker::slot_launching);
    // stopping
    connect(this, &db_manager::signal_stopping_workers,
            ptr_db_system_worker, &db_system_worker::slot_stopping);

    ptr_db_system_thread->start();
}

void db_manager::create_db_writer_worker(db_state_workers *state)
{
    ptr_db_writer_worker = new db_writer_worker;
    ptr_db_writer_worker->set_configuration(m_settings);

    // add "db_writer_worker" to state monitor
    state->add_name_workers(ptr_db_writer_worker->metaObject()->className());

    ptr_db_writer_thread = new QThread;
    ptr_db_writer_worker->moveToThread(ptr_db_writer_thread);

    // initialization
    connect(this, &db_manager::signal_initialization_workers,
            ptr_db_writer_worker, &db_writer_worker::slot_initialization);
    // launching
    connect(this, &db_manager::signal_launching_workers,
            ptr_db_writer_worker, &db_writer_worker::slot_launching);
    // stopping
    connect(this, &db_manager::signal_stopping_workers,
            ptr_db_writer_worker, &db_writer_worker::slot_stopping);

    ptr_db_writer_thread->start();
}

void db_manager::create_db_path_monitor_worker(db_state_workers *state)
{
    // db path monitor
    ptr_db_path_monitor_worker = new db_path_monitor;
    ptr_db_path_monitor_worker->set_configuration(m_settings);

    // add "db_path_monitor" to state monitor
    state->add_name_workers(ptr_db_path_monitor_worker->metaObject()->className());

    ptr_db_path_monitor_thread = new QThread;
    ptr_db_path_monitor_worker->moveToThread(ptr_db_path_monitor_thread);

    // initialization
    connect(this, &db_manager::signal_initialization_workers,
            ptr_db_path_monitor_worker, &db_path_monitor::slot_initialization);
    // launching
    connect(this, &db_manager::signal_launching_workers,
            ptr_db_path_monitor_worker, &db_path_monitor::slot_launching);
    // stopping
    connect(this, &db_manager::signal_stopping_workers,
            ptr_db_path_monitor_worker, &db_path_monitor::slot_stopping);

    ptr_db_path_monitor_thread->start();
}
