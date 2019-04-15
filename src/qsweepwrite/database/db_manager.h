#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QObject>
#include <QPointer>

#include "sweep_write_settings.h"
#include "db_state_workers.h"
#include "db_reader_worker.h"
#include "db_writer_worker.h"
#include "db_path_monitor.h"

class db_manager : public QObject
{
    Q_OBJECT
public:
    explicit db_manager(QObject *parent = nullptr);

    void set_configuration(const sweep_write_settings &);

    void initialization();
    void launching();
    void stopping();

public slots:
    void slot_is_all_initialization_workers();
    void slot_is_all_launching_workers();
    void slot_is_all_stopping_workers();

    void slot_received_data(const QByteArray &);

signals:
    void signal_initialization_workers();
    void signal_launching_workers();
    void signal_stopping_workers();

public slots:

private:
    bool is_ready;
    sweep_write_settings m_settings;

    // db state workers
    db_state_workers *ptr_db_state_workers {Q_NULLPTR};

    // db write workers
    db_writer_worker *ptr_db_writer_worker {Q_NULLPTR};
    QPointer<QThread> ptr_db_writer_thread;
    void create_db_writer_worker(db_state_workers *state);

    // db path monitor
    db_path_monitor *ptr_db_path_monitor_worker {Q_NULLPTR};
    QPointer<QThread> ptr_db_path_monitor_thread;
    void create_db_path_monitor_worker(db_state_workers *state);
};

#endif // DB_MANAGER_H
