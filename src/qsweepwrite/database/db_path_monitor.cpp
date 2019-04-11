#include "db_path_monitor.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_path_monitor::db_path_monitor(QObject *parent) : QObject(parent)
{

}

void db_path_monitor::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void db_path_monitor::initialization()
{

}

void db_path_monitor::launching()
{

}

void db_path_monitor::stopping()
{

}
