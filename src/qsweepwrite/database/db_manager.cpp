#include "db_manager.h"

#include <QThread>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_manager::db_manager(QObject *parent) : QObject(parent)
{
    is_ready = false;

    qRegisterMetaType<state_workers_type>();

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
    is_ready = false;

#ifdef QT_DEBUG
    qDebug().noquote() << tr("all initialization workers");
#endif

    emit signal_launching_workers();
}

void db_manager::slot_is_all_launching_workers()
{
    is_ready = true;

#ifdef QT_DEBUG
    qDebug().noquote() << tr("---------------------");
    qDebug().noquote() << tr("all launching workers");
    qDebug().noquote() << tr("---------------------");
#endif
}

void db_manager::slot_is_all_stopping_workers()
{
    is_ready = false;

#ifdef QT_DEBUG
    qDebug().noquote() << tr("all stopping workers");
#endif
}

void db_manager::slot_received_data(const QByteArray &rc_data)
{
    Q_UNUSED(rc_data)

    if(is_ready)
    {

    }
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
    // state workers
    connect(ptr_db_writer_worker, &db_writer_worker::signal_update_state_workers,
            state, &db_state_workers::slot_update_state_workers);

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
    // state workers
    connect(ptr_db_path_monitor_worker, &db_path_monitor::signal_update_state_workers,
            state, &db_state_workers::slot_update_state_workers);

    ptr_db_path_monitor_thread->start();
}
