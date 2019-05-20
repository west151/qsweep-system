#include "db_state_workers.h"

#include "db_const.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_state_workers::db_state_workers(QObject *parent) : QObject(parent)
{
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

#ifdef QT_DEBUG
        qDebug() << db_name << format_size(db_size) << "(" << db_size << ")";
#endif
}

void db_state_workers::slot_state_db(const QString &db_name, const state_db &state)
{
    m_state_db.insert(db_name, state);

#ifdef QT_DEBUG
        qDebug() << db_name << "(" << state << ")";
#endif
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
        emit signal_all_launching();
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
