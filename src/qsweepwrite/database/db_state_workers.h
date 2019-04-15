#ifndef DB_STATE_WORKERS_H
#define DB_STATE_WORKERS_H

#include <QObject>
#include <QMap>

enum state_workers_type: qint32 {
    unknown,
    initialization,
    launching,
    stopping
};

Q_DECLARE_METATYPE(state_workers_type)

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

public slots:
    void slot_update_state_workers(const state_workers_type &type);

private:
   QMap<QString, state_workers_type> m_workers_state;

   void is_all_initialization();
   void is_all_launching();
   void is_all_stopping();
};

#endif // DB_STATE_WORKERS_H
