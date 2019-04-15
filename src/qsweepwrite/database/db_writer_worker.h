#ifndef DB_WRITER_WORKER_H
#define DB_WRITER_WORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "sweep_write_settings.h"
#include "db_state_workers.h"

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

signals:
    void signal_update_state_workers(const state_workers_type &type);

private:
    QSqlDatabase m_dbase;
    QString m_str_error_dbase;
    QMap <QString, bool> m_init_db_status;
    QMap <QString, qint64> m_db_size;

    sweep_write_settings m_settings;

    void open_db(const QString &);
    bool start_transaction();
    bool commit_transaction();
    void set_pragma(const QString &, const QString &);
    bool is_table_name_resolve(const QString &);
    void update_last_error(QSqlQuery* query);
    void close_db();
};

#endif // DB_WRITER_WORKER_H
