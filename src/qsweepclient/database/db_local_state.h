#ifndef DB_LOCAL_STATE_H
#define DB_LOCAL_STATE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMap>

#include "params_spectr.h"

class db_local_state : public QObject
{
    Q_OBJECT
public:
    explicit db_local_state(QObject *parent = nullptr);

signals:
    void signal_read_params_spectr(const QVector<params_spectr> &);
    void signal_last_error(const QString &);

public slots:
    void slot_open_db(const QString &);
    void slot_write_params_spectr(const QVector<params_spectr> &);
    void slot_update_params_spectr(const QVector<params_spectr> &);
    void slot_remove_params_spectr(const QString &id_params);
    void slot_read_params_spectr();
    void slot_close_db();

private:
    QSqlDatabase m_dbase;
    QString m_str_error_dbase;
    QMap<QString, QMap<QString, QString> > m_table;
    QMap<QString, QString> m_column_visual_settings;

    bool create_table(const QString &)const;
    bool is_table_name_resolve(const QString &);
    QString list_column_and_type(const QString &)const;
    QString list_column(const QString &prefix = "");
    void set_pragma(const QString &, const QString &);
    bool start_transaction();
    bool commit_transaction();
    void update_last_error(QSqlQuery* query);
};

#endif // DB_LOCAL_STATE_H
