#ifndef STATE_WORKER_H
#define STATE_WORKER_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class state_worker_data;

class state_worker
{
public:
    state_worker();
    state_worker(const state_worker &);
    state_worker &operator=(const state_worker &);
    ~state_worker();

    void set_worker_name(const QString &);
    QString worker_name()const;

private:
    QSharedDataPointer<state_worker_data> data;
};

Q_DECLARE_METATYPE(state_worker)

#endif // STATE_WORKER_H
