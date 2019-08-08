#ifndef PARSER_WORKER_H
#define PARSER_WORKER_H

#include <QObject>

#include "data_spectr.h"

class parser_worker : public QObject
{
    Q_OBJECT
public:
    explicit parser_worker(QObject *parent = nullptr);

signals:
    void signal_data_spectr_message(const QByteArray &);

public slots:
    void slot_input_line(const QByteArray &);

private:
    bool is_parser_range;
    bool is_complete_parser_range;
    QVector<power_spectr> buffer_power_db;
};

#endif // SWEEP_PARSER_WORKER_H
