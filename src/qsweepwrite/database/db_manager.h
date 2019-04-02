#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QObject>

#include "sweep_write_settings.h"

class db_manager : public QObject
{
    Q_OBJECT
public:
    explicit db_manager(QObject *parent = nullptr);

    void set_configuration(const sweep_write_settings &);
    void initialization();
    void launching();
    void stopping();

signals:

public slots:

private:
    sweep_write_settings *ptr_sweep_write_settings {Q_NULLPTR};
};

#endif // DB_MANAGER_H
