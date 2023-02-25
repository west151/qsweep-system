#ifndef SPECTRUM_PROCESS_WORKER_H
#define SPECTRUM_PROCESS_WORKER_H

#include <QObject>
#include <QProcess>

#include "params_spectr.h"

class spectrum_process_worker : public QObject
{
    Q_OBJECT
public:
    explicit spectrum_process_worker(const QString &file_name = "", QObject *parent = nullptr);

public slots:
    void slot_run_process_worker(const QByteArray &value);
    void slot_stop_process_worker();

private slots:
    void slot_started();
    void slot_ready_read_standard_output();
    void slot_ready_read_standard_error();
    void slot_finished(int exit_code, QProcess::ExitStatus exit_status);

signals:
    void signal_message(const QByteArray &);
    void signal_output_line(const QByteArray &);
    void signal_state();

private:
    bool is_ready;
    QProcess* ptr_process_hackrf_sweep {Q_NULLPTR};

    void send_message_log(const QString &value);
    QStringList make_argument_list(const params_spectr &);

    void read_test_data_from_file();
    void save_test_data_to_file(const QString &);
};

#endif // SPECTRUM_PROCESS_WORKER_H
