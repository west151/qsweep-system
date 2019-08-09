#ifndef SPECTRUM_PROCESS_WORKER_H
#define SPECTRUM_PROCESS_WORKER_H

#include <QObject>

class spectrum_process_worker : public QObject
{
    Q_OBJECT
public:
    explicit spectrum_process_worker(QObject *parent = nullptr);

public slots:
    void slot_run_process_worker(const QByteArray &value);
    void slot_stop_process_worker();

signals:
    void signal_output_line(const QByteArray &);

private:
    bool is_ready;

    void read_test_data_from_file();
};

#endif // SPECTRUM_PROCESS_WORKER_H
