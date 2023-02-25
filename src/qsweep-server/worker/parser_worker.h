#ifndef PARSER_WORKER_H
#define PARSER_WORKER_H

#include <QObject>

#include "data_spectr.h"

class parser_worker : public QObject
{
    Q_OBJECT
public:
    explicit parser_worker(QObject *parent = nullptr);

public slots:
    void slot_input_line(const QByteArray &);
    void slot_run_parser_worker(const QByteArray &);
    void slot_stop_parser_worker();

signals:
    void signal_data_spectr_message(const QByteArray &);
    void signal_run_process_worker(const QByteArray &);

private:
    bool is_parser_range;
    bool is_complete_parser_range;
    bool is_run_parser;
    QVector<power_spectr> buffer_power_db;
    quint64 hz_low_run_process;
    quint64 hz_high_run_process;
    QString id_params_str;
};

#endif // SWEEP_PARSER_WORKER_H
