#ifndef DB_WRITER_WORKER_H
#define DB_WRITER_WORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "sweep_write_settings.h"
#include "db_state_workers.h"
#include "data_spectr.h"
#include "params_spectr.h"

class db_writer_worker : public QObject
{
    Q_OBJECT
public:
    explicit db_writer_worker(QObject *parent = nullptr);

    void set_configuration(const sweep_write_settings &);

public slots:
    void slot_initialization();
    void slot_launching();
    void slot_stopping();

    void slot_data_to_write(const QByteArray &);

signals:
    void signal_update_state_workers(const state_workers &);
    void signal_file_size(const QString &, const qint64 &);
    void signal_state_db(const QString &, const state_db &);

private:
    QSqlDatabase m_dbase;
    QString m_str_error_dbase;
    params_spectr m_params_spectr_to_write;

    QMap <QString, bool> m_init_db_file_status;
    QMap <QString, qint64> m_db_file_size;
    QMap <QString, state_db> m_db_file_state;

    sweep_write_settings m_settings;

    void open_db(const QString &);
    bool start_transaction();
    bool commit_transaction();
    void set_pragma(const QString &, const QString &);
    bool is_table_name_resolve(const QString &);
    bool create_table(const QString &)const;
    void update_last_error(QSqlQuery* query);    
    void data_spectr_to_write(const data_spectr &);
    void data_params_to_write(const params_spectr &);
    void close_db();

    void update_size_file(const QString &);
    QString file_selection_for_writing()const;
};

#endif // DB_WRITER_WORKER_H
