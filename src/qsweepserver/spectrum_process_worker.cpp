#include "spectrum_process_worker.h"
#include "sweep_message.h"
#include "params_spectr.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTime>
#include <unistd.h>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

spectrum_process_worker::spectrum_process_worker(QObject *parent) : QObject(parent),
    is_ready(true)
{

}

void spectrum_process_worker::slot_run_process_worker(const QByteArray &value)
{
    if(is_ready)
    {
        const sweep_message ctrl_info(value);
        const params_spectr params_spectr_data(ctrl_info.data_message());

        // for test
        read_test_data_from_file();
    }
}

void spectrum_process_worker::slot_stop_process_worker()
{

}

void spectrum_process_worker::read_test_data_from_file()
{

#ifdef QT_DEBUG
    qDebug() << QCoreApplication::applicationFilePath();
#endif

    // parser_data_input
    // hackrf_sweep -f 2300:2700 -l 32 -g 20 -w 125000 -r 2300_2700_125000.out
    // hackrf_sweep -f 2300:2700 -l 32 -g 20 -w 100000 -r 2300_2700_100000.out

    QFileInfo info(QCoreApplication::applicationFilePath());
    QString file_test_data(info.absolutePath()
                           + QDir::separator()
                           + "parser_data_input"
                           + QDir::separator()
                           + "2300_2700_125000_v1.out");

    QFile input_file(file_test_data);
    input_file.open(QIODevice::ReadOnly);

    if (!input_file.isOpen())
        return;

    QTextStream stream(&input_file);

    QString line;
    while (stream.readLineInto(&line))
    {
        QByteArray send_data;
        send_data.append(line);
        signal_output_line(send_data);

        usleep(200);
    }

    if(input_file.isOpen())
        input_file.close();
}
