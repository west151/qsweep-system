#include "top_model.h"
#include <QTimer>

top_model::top_model(QObject *parent) : QObject(parent),
    m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout,
            this, &top_model::slot_system_time_update);
}

void top_model::start()
{
    m_timer->start(1000);
}

QString top_model::system_time() const
{
    return m_system_time.toString("hh:mm:ss");
}

void top_model::slot_system_time_update()
{
    m_system_time = QDateTime::currentDateTime();

    emit signal_time_changed();
}
