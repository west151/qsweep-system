#ifndef SWEEP_WRITE_SETTINGS_H
#define SWEEP_WRITE_SETTINGS_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>



class sweep_write_settings_data;

class sweep_write_settings
{
public:
    sweep_write_settings();
    sweep_write_settings(const sweep_write_settings &);
    sweep_write_settings(const QByteArray &json, const bool binary = false);
    sweep_write_settings &operator=(const sweep_write_settings &);
    ~sweep_write_settings();

    bool is_valid() const;

    void set_host_broker(const QString &);
    QString host_broker()const;

    void set_port_broker(const quint16 &);
    quint16 port_broker()const;

    void set_delayed_launch(const int &);
    int delayed_launch()const;

    void set_db_path(const QString &);
    QString db_path()const;

    void set_db_file_count(const qint32 &);
    qint32 db_file_count()const;

    void set_db_file_size(const qint32 &);
    qint32 db_file_size()const;

    void set_backup_path(const QString &);
    QString backup_path()const;

    void set_data_backup(const bool &);
    bool data_backup()const;

    void set_backup_compress_level(const int &);
    int backup_compress_level()const;

    QByteArray to_json(const bool binary = false, const bool isCompact = true) const;

private:
    QSharedDataPointer<sweep_write_settings_data> data;
};

Q_DECLARE_METATYPE(sweep_write_settings)

#endif // SWEEP_WRITE_SETTINGS_H
