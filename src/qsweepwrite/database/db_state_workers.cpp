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
            m_workers_state.insert(name, type);
    }
}
