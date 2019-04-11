#include "db_path_monitor.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_path_monitor::db_path_monitor(QObject *parent) : QObject(parent)
{
    setObjectName(this->metaObject()->className());
}

void db_path_monitor::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void db_path_monitor::slot_initialization()
{
#ifdef QT_DEBUG
        qDebug().noquote() << tr("className:") << this->metaObject()->className();
#endif
}

void db_path_monitor::slot_launching()
{

}

void db_path_monitor::slot_stopping()
{

}

