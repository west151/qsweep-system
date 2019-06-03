#include "db_state_workers.h"

#include <QTime>

#include "db_const.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_state_workers::db_state_workers(QObject *parent) : QObject(parent)
{
    m_timer_writed = new QTime;
}

void db_state_workers::add_name_workers(const QString &name)
{
    m_workers_state.insert(name, state_workers::unknown);
}

void db_state_workers::slot_update_state_workers(const state_workers &type)
{
    QObject* sender_state = qobject_cast<QObject*>(sender());

    if(sender_state != Q_NULLPTR)
    {
        const auto name = sender_state->objectName();

        if(!name.isEmpty())
        {
            m_workers_state.insert(name, type);

            is_all_initialization();

            is_all_launching();

            is_all_stopping();
        }
    }
}

void db_state_workers::slot_db_size(const QString &db_name, const qint64 &db_size)
{
    m_db_file_size.insert(db_name, db_size);

    if(m_timer_writed->elapsed()>=5000)
    {

#ifdef QT_DEBUG
        qDebug() << db_name << format_size(db_size) << "(" << db_size << ")"
                 << QString("Time elapsed: %1 ms, %2 s").arg(m_timer_writed->elapsed()).arg(m_timer_writed->elapsed()/1000);
#endif

        m_timer_writed->restart();
    }
}

void db_state_workers::slot_state_db(const QString &db_name, const state_db &state)
{
    m_state_db.insert(db_name, state);

    // file backup
    if((state==state_db::file_is_full)&&(m_state_db.contains(db_name)))
        emit signal_file_to_backup(db_name);

    // file data clean
    if((state==state_db::file_is_backup)&&(m_state_db.contains(db_name)))
        emit signal_start_cleaner(db_name);

    // file is ready
    if((state==state_db::file_is_ready)&&(m_state_db.contains(db_name)))
        emit signal_file_is_ready(db_name);
}

void db_state_workers::is_all_initialization()
{
    const auto list_state = m_workers_state.values();
    qint16 counter = 0;

    for(int i=0; i<list_state.size(); ++i)
        if(list_state.at(i)==state_workers::initialization)
            counter++;

    if(counter==list_state.size())
        emit signal_all_initialization();
}

void db_state_workers::is_all_launching()
{
    const auto list_state = m_workers_state.values();
    qint16 counter = 0;

    for(int i=0; i<list_state.size(); ++i)
        if(list_state.at(i)==state_workers::launching)
            counter++;

    if(counter==list_state.size())
    {
        m_timer_writed->start();
        emit signal_all_launching();
    }

//    // for test
//    if(counter==list_state.size())
//    {
//        const auto list_file_is_full = m_state_db.keys(state_db::file_is_full);

//#ifdef QT_DEBUG
//        qDebug() << "File is full:" << list_file_is_full;
//#endif
//        emit signal_start_cleaner(list_file_is_full.first());
//    }
}

void db_state_workers::is_all_stopping()
{
    const auto list_state = m_workers_state.values();
    qint16 counter = 0;

    for(int i=0; i<list_state.size(); ++i)
        if(list_state.at(i)==state_workers::stopping)
            counter++;

    if(counter==list_state.size())
        emit signal_all_stopping();
}
