#include "db_manager.h"

db_manager::db_manager(QObject *parent) : QObject(parent)
{

}

void db_manager::set_configuration(const sweep_write_settings &settings)
{
    ptr_sweep_write_settings = new sweep_write_settings(settings);
}

void db_manager::initialization()
{

}

void db_manager::launching()
{

}

void db_manager::stopping()
{

}
