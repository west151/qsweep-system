#include "state_worker.h"

class state_worker_data : public QSharedData {
public:
    state_worker_data(): QSharedData()
    {
        worker_name.clear();
    }
    state_worker_data(const state_worker_data &other) : QSharedData(other)
    {
        worker_name = other.worker_name;
    }

    ~state_worker_data() {}

    QString worker_name;
};

state_worker::state_worker() : data(new state_worker_data)
{
}

state_worker::state_worker(const state_worker &rhs) : data(rhs.data)
{
}

state_worker &state_worker::operator=(const state_worker &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

state_worker::~state_worker()
{
}

void state_worker::set_worker_name(const QString &value)
{
    data->worker_name = value;
}

QString state_worker::worker_name() const
{
    return data->worker_name;
}
