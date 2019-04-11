#include "db_system_worker.h"
#include <QSqlRecord>
#include <QSqlError>

#include "db_const.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_system_worker::db_system_worker(QObject *parent) : QObject(parent)
{
    setObjectName(this->metaObject()->className());
}

void db_system_worker::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void db_system_worker::slot_initialization()
{
#ifdef QT_DEBUG
        qDebug().noquote() << tr("className:") << this->metaObject()->className();
#endif
}

void db_system_worker::slot_launching()
{

}

void db_system_worker::slot_stopping()
{

}
