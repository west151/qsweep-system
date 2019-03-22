#ifndef CORE_SWEEP_WRITE_H
#define CORE_SWEEP_WRITE_H

#include <QObject>

class sweep_write_settings;

class core_sweep_write : public QObject
{
    Q_OBJECT
public:
    explicit core_sweep_write(const QString &, QObject *parent = nullptr);

signals:

public slots:

private:
    sweep_write_settings *ptr_sweep_write_settings {Q_NULLPTR};

    void initialization();
    void launching();

    bool read_settings(const QString &);
    bool save_settings(const QString &);
};

#endif // CORE_SWEEP_WRITE_H
