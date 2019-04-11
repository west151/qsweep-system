#include "db_writer_worker.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_writer_worker::db_writer_worker(QObject *parent) : QObject(parent)
{
    setObjectName(this->metaObject()->className());
}

void db_writer_worker::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void db_writer_worker::slot_initialization()
{
#ifdef QT_DEBUG
        qDebug().noquote() << tr("className:") << this->metaObject()->className();
#endif
}

void db_writer_worker::slot_launching()
{

}

void db_writer_worker::slot_stopping()
{

}
