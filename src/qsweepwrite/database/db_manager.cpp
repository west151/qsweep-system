#include "db_manager.h"

#include <QThread>
#include <QTimer>
#include <QStorageInfo>

#include "sweep_message.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_manager::db_manager(QObject *parent) : QObject(parent)
{
    is_ready = false;

    qRegisterMetaType<state_workers>();
    qRegisterMetaType<state_db>();

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

//    auto timer = new QTimer();
//    connect(timer, SIGNAL(timeout()),
//            this, SLOT(slot_test_received_data()));
//    timer->start(500);
}

void db_manager::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void db_manager::initialization()
{
    QDir dir(m_settings.db_path());

    if (!dir.exists())
        dir.mkpath(".");

    if (dir.exists())
    {
        // remove all files
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);

        for(const QString &dirFile: dir.entryList())
            dir.remove(dirFile);
    }

    QStorageInfo storage(m_settings.db_path());

    if (storage.isValid() && storage.isReady())
    {
        // calc need free disk space (Mb)
        qint64 need_size = m_settings.db_file_size()*m_settings.db_file_count();
        qint64 available_size = storage.bytesAvailable()/1024/1024;

#ifdef QT_DEBUG
        qDebug() << tr("StorageInfo") << tr("name:") << storage.name();
        qDebug() << tr("StorageInfo") << tr("file system type:") << storage.fileSystemType();
        qDebug() << tr("StorageInfo") << tr("size:") << storage.bytesTotal()/1000/1000 << "MB";
        qDebug() << tr("StorageInfo") << tr("available size:") << available_size << "MB";
        qDebug() << tr("StorageInfo") << tr("need free disk space:") << need_size << "MB";
#endif
        if(available_size > need_size)
        {
            // db writer
            create_db_writer_worker(ptr_db_state_workers);
            // db cleaner
            create_db_cleaner_worker(ptr_db_state_workers);

            if(m_settings.data_backup())
                create_file_backup_worker(ptr_db_state_workers);

            emit signal_initialization_workers();

        }else{
#ifdef QT_DEBUG
            qCritical() << tr("StorageInfo")
                        << tr("available size:") << available_size << "MB"
                        << tr("need free disk space:") << need_size << "MB";
#endif
        }
    }
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
    qDebug() << tr("all initialization workers");
#endif

    emit signal_launching_workers();
}

void db_manager::slot_is_all_launching_workers()
{
    is_ready = true;

#ifdef QT_DEBUG
    qDebug() << tr("all launching workers");
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
    if(is_ready)
        emit signal_send_data_to_write(rc_data);
}

void db_manager::slot_test_received_data()
{
    sweep_message send_data;
    send_data.set_type(type_message::data_spectr);

    QVector<power_spectr> powers;

    for(int q=0; q<40; ++q)
    {
        power_spectr p1;
        p1.m_date_time = QDateTime::currentDateTimeUtc();
        p1.m_fft_bin_width = 250000;
        p1.m_fft_size = 100;
        p1.m_frequency_min = 100000000;
        p1.m_frequency_max = 200000000;

        for(int i=0; i<100; ++i){
            qreal value = rm.generateDouble()*-100;
            p1.m_power.append(value);
        }

        powers.append(p1);
    }

    data_spectr spectr;
    spectr.set_spectr(powers);

    send_data.set_data_message(spectr.to_json());

    slot_received_data(send_data.export_json());
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
    // monitor db file size
    connect(ptr_db_writer_worker, &db_writer_worker::signal_file_size,
            state, &db_state_workers::slot_db_size);
    // monitor state db (file)
    connect(ptr_db_writer_worker, &db_writer_worker::signal_state_db,
            state, &db_state_workers::slot_state_db);
    // state file is ready
    connect(state, &db_state_workers::signal_file_is_ready,
            ptr_db_writer_worker, &db_writer_worker::slot_file_is_ready);

    // send data to write db
    connect(this, &db_manager::signal_send_data_to_write,
            ptr_db_writer_worker, &db_writer_worker::slot_data_to_write);

    ptr_db_writer_thread->start();
}

void db_manager::create_db_cleaner_worker(db_state_workers *state)
{
    ptr_db_cleaner_workers = new db_cleaner_workers;
    ptr_db_cleaner_workers->set_configuration(m_settings);

    // add "db_cleaner_workers" to state monitor
    state->add_name_workers(ptr_db_cleaner_workers->metaObject()->className());

    // initialization
    connect(this, &db_manager::signal_initialization_workers,
            ptr_db_cleaner_workers, &db_cleaner_workers::slot_initialization);
    // launching
    connect(this, &db_manager::signal_launching_workers,
            ptr_db_cleaner_workers, &db_cleaner_workers::slot_launching);
    // stopping
    connect(this, &db_manager::signal_stopping_workers,
            ptr_db_cleaner_workers, &db_cleaner_workers::slot_stopping);

    // state workers
    connect(ptr_db_cleaner_workers, &db_cleaner_workers::signal_update_state_workers,
            state, &db_state_workers::slot_update_state_workers);
    // delete table
    connect(state, &db_state_workers::signal_start_cleaner,
            ptr_db_cleaner_workers, &db_cleaner_workers::slot_clean_db);
    //
    connect(ptr_db_cleaner_workers, &db_cleaner_workers::signal_state_db,
            state, &db_state_workers::slot_state_db);

    ptr_db_cleaner_thread = new QThread;
    ptr_db_cleaner_workers->moveToThread(ptr_db_cleaner_thread);

    ptr_db_cleaner_thread->start();
}

void db_manager::create_file_backup_worker(db_state_workers *state)
{
    ptr_file_backup_workers = new file_backup_workers;
    ptr_file_backup_workers->set_configuration(m_settings);

    ptr_file_backup_thread = new QThread;
    ptr_file_backup_workers->moveToThread(ptr_file_backup_thread);

    // add "file_backup_workers" to state monitor
    state->add_name_workers(ptr_file_backup_workers->metaObject()->className());

    // initialization
    connect(this, &db_manager::signal_initialization_workers,
            ptr_file_backup_workers, &file_backup_workers::slot_initialization);
    // launching
    connect(this, &db_manager::signal_launching_workers,
            ptr_file_backup_workers, &file_backup_workers::slot_launching);
    // stopping
    connect(this, &db_manager::signal_stopping_workers,
            ptr_file_backup_workers, &file_backup_workers::slot_stopping);

    // state workers
    connect(ptr_file_backup_workers, &file_backup_workers::signal_update_state_workers,
            state, &db_state_workers::slot_update_state_workers);

    connect(state, &db_state_workers::signal_file_to_backup,
            ptr_file_backup_workers, &file_backup_workers::slot_file_backup);

    // state file db
    connect(ptr_file_backup_workers, &file_backup_workers::signal_state_db,
            state, &db_state_workers::slot_state_db);

    ptr_file_backup_thread->start();
}
