#ifndef DB_WRITER_WORKER_H
#define DB_WRITER_WORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "sweep_write_settings.h"

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
    void signal_is_initialization(const bool &);
    void signal_is_launching(const bool &);
    void signal_is_stopping(const bool &);

private:
    QSqlDatabase m_dbase;
    QString m_str_error_dbase;

    sweep_write_settings m_settings;
};

#endif // DB_WRITER_WORKER_H
