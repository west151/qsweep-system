#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QObject>
#include <QPointer>
#include <QRandomGenerator>

#include "sweep_write_settings.h"
#include "db_state_workers.h"
#include "db_reader_worker.h"
#include "db_writer_worker.h"
#include "db_cleaner_workers.h"
#include "file_backup_workers.h"

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

    void slot_test_received_data();

signals:
    void signal_initialization_workers();
    void signal_launching_workers();
    void signal_stopping_workers();

    void signal_send_data_to_write(const QByteArray &);

private:
    bool is_ready;
    sweep_write_settings m_settings;    

    // db state workers
    db_state_workers *ptr_db_state_workers {Q_NULLPTR};

    // db write workers
    db_writer_worker *ptr_db_writer_worker {Q_NULLPTR};
    QPointer<QThread> ptr_db_writer_thread;
    void create_db_writer_worker(db_state_workers *state);

    // db clear workers
    db_cleaner_workers *ptr_db_cleaner_workers {Q_NULLPTR};
    QPointer<QThread> ptr_db_cleaner_thread;
    void create_db_cleaner_worker(db_state_workers *state);

    // db file backup
    file_backup_workers *ptr_file_backup_workers {Q_NULLPTR};
    QPointer<QThread> ptr_file_backup_thread;
    void create_file_backup_worker(db_state_workers *state);

    // test
    QRandomGenerator rm;
};

#endif // DB_MANAGER_H
