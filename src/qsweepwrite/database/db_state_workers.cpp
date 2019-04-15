#include "db_state_workers.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_state_workers::db_state_workers(QObject *parent) : QObject(parent)
{
}

void db_state_workers::add_name_workers(const QString &name)
{
    m_workers_state.insert(name, state_workers_type::unknown);
}

void db_state_workers::slot_update_state_workers(const state_workers_type &type)
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

void db_state_workers::is_all_initialization()
{
    const auto list_state = m_workers_state.values();
    qint16 counter = 0;

    for(int i=0; i<list_state.size(); ++i)
        if(list_state.at(i)==state_workers_type::initialization)
            counter++;

    if(counter==list_state.size())
        emit signal_all_initialization();
}

void db_state_workers::is_all_launching()
{
    const auto list_state = m_workers_state.values();
    qint16 counter = 0;

    for(int i=0; i<list_state.size(); ++i)
        if(list_state.at(i)==state_workers_type::launching)
            counter++;

    if(counter==list_state.size())
        emit signal_all_launching();
}

void db_state_workers::is_all_stopping()
{
    const auto list_state = m_workers_state.values();
    qint16 counter = 0;

    for(int i=0; i<list_state.size(); ++i)
        if(list_state.at(i)==state_workers_type::stopping)
            counter++;

    if(counter==list_state.size())
        emit signal_all_stopping();
}
