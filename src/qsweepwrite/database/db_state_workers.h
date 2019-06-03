#ifndef DB_STATE_WORKERS_H
#define DB_STATE_WORKERS_H

#include <QObject>
#include <QMap>

enum state_workers: qint32 {
    unknown,
    initialization,
    launching,
    stopping,
    error
};

Q_DECLARE_METATYPE(state_workers)

enum state_db: qint32 {
    file_unknown,
    file_write,
    file_read,
    file_clean,
    file_is_full,
    file_is_ready,
    file_is_backup
};

Q_DECLARE_METATYPE(state_db)

class db_state_workers : public QObject
{
    Q_OBJECT
public:
    explicit db_state_workers(QObject *parent = nullptr);

    void add_name_workers(const QString &name);

signals:
    void signal_all_initialization();
    void signal_all_launching();
    void signal_all_stopping();
    void signal_start_cleaner(const QString &);
    void signal_file_to_backup(const QString &);
    void signal_file_is_ready(const QString &);

public slots:
    void slot_update_state_workers(const state_workers &type);
    void slot_db_size(const QString &, const qint64 &);
    void slot_state_db(const QString &, const state_db &);

private:
    QTime *m_timer_writed {Q_NULLPTR};
    QMap<QString, state_workers> m_workers_state;
    QMap <QString, qint64> m_db_file_size;
    QMap <QString, state_db> m_state_db;

    void is_all_initialization();
    void is_all_launching();
    void is_all_stopping();
};

#endif // DB_STATE_WORKERS_H
