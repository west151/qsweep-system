#include "top_model.h"
#include <QTimer>

top_model::top_model(QObject *parent) : QObject(parent),
    m_timer(new QTimer(this))
{
    m_connect_brocker = false;

    connect(m_timer, &QTimer::timeout,
            this, &top_model::slot_system_time_update);

    // default datetime
    m_system_time = QDateTime::currentDateTime();
}

void top_model::start()
{
    m_timer->start(1000);
}

QString top_model::system_time() const
{
    return m_system_time.toString("hh:mm:ss");
}

bool top_model::is_connect_brocker()
{
    return m_connect_brocker;
}

void top_model::slot_set_connect_brocker(bool on)
{
    m_connect_brocker = on;

    emit signal_connect_brocker_changed();
}

void top_model::slot_system_time_update()
{
    m_system_time = QDateTime::currentDateTime();

    emit signal_time_changed();
}
