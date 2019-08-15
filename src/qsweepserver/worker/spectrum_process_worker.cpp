#include "spectrum_process_worker.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTime>
#include <unistd.h>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

#include "sweep_message.h"
#include "data_log.h"

spectrum_process_worker::spectrum_process_worker(const QString &file_name, QObject *parent) : QObject(parent),
    is_ready(true),
    ptr_process_hackrf_sweep(new QProcess(this))
{
    if(!file_name.isEmpty())
    {
        ptr_process_hackrf_sweep->setProgram(file_name);        

        connect(ptr_process_hackrf_sweep, &QProcess::started,
                this, &spectrum_process_worker::slot_started);

        connect(ptr_process_hackrf_sweep, &QProcess::readyReadStandardOutput,
                this, &spectrum_process_worker::slot_ready_read_standard_output);

        connect(ptr_process_hackrf_sweep, &QProcess::readyReadStandardError,
                this, &spectrum_process_worker::slot_ready_read_standard_error);

        connect(ptr_process_hackrf_sweep, &QProcess::finished,
                this, &spectrum_process_worker::slot_finished);

    }
}

void spectrum_process_worker::slot_run_process_worker(const QByteArray &value)
{
    if(is_ready)
    {
        const sweep_message ctrl_info(value);
        const params_spectr params_spectr_data(ctrl_info.data_message());
        const auto arguments = make_argument_list(params_spectr_data);

        qDebug() << arguments;

        // for test
        //read_test_data_from_file();

        // start spectr
        if(params_spectr_data.start_spectr())
        {
            if(ptr_process_hackrf_sweep->state()==QProcess::Running)
                ptr_process_hackrf_sweep->kill();

            ptr_process_hackrf_sweep->setArguments(arguments);
            ptr_process_hackrf_sweep->start(QProcess::ReadOnly); //QProcess::Unbuffered  //

            if(ptr_process_hackrf_sweep->waitForStarted(1000)){
                send_message_log(QString("Environment: %1").arg(ptr_process_hackrf_sweep->environment().join(",")));
                send_message_log(QString("Program %1 started").arg(ptr_process_hackrf_sweep->program()));
            }else
                send_message_log(QString("Can't start program %1").arg(ptr_process_hackrf_sweep->program()));
        }

        //        // stop spectr
        //        if(!params_spectr_data.start_spectr())
        //        {
        //            if(ptr_process_hackrf_sweep->state()==QProcess::Running)
        //                ptr_process_hackrf_sweep->kill();
        //        }
    }
}

void spectrum_process_worker::slot_stop_process_worker()
{
    if(ptr_process_hackrf_sweep->state()==QProcess::Running)
        ptr_process_hackrf_sweep->kill();
}

void spectrum_process_worker::slot_started()
{
#ifdef QT_DEBUG
    qDebug() << "Program started:" << ptr_process_hackrf_sweep->program();
#endif
}

void spectrum_process_worker::slot_ready_read_standard_output()
{
    ptr_process_hackrf_sweep->setReadChannel(QProcess::StandardOutput);

    while (ptr_process_hackrf_sweep->canReadLine()) {
        QByteArray ba(ptr_process_hackrf_sweep->readLine());
        emit signal_output_line(ba);
    }
}

void spectrum_process_worker::slot_ready_read_standard_error()
{
//#ifdef QT_DEBUG
//    qDebug() << "Program:" << ptr_process_hackrf_sweep->program()
//             << "string error:" << ptr_process_hackrf_sweep->readAllStandardError();
//#endif

    ptr_process_hackrf_sweep->setReadChannel(QProcess::StandardError);
    QTextStream stream(ptr_process_hackrf_sweep);

    while (!stream.atEnd()) {
                QString line = stream.readLine();
                send_message_log(line);
    }
}

void spectrum_process_worker::slot_finished(int exit_code, QProcess::ExitStatus exit_status)
{
#ifdef QT_DEBUG
    qDebug() << "Program finished:" << ptr_process_hackrf_sweep->program()
             << "exit_code:" << exit_code
             << "exit_status:" << exit_status;
#endif
}

void spectrum_process_worker::send_message_log(const QString &value)
{
    data_log message;
    message.set_text_message(value);

    sweep_message send_data;
    send_data.set_type(type_message::data_message_log);
    send_data.set_data_message(message.export_json());

    emit signal_message(send_data.export_json());
}

QStringList spectrum_process_worker::make_argument_list(const params_spectr &params)
{
    QStringList argument_list;

    argument_list << "-f"
                  << QString::number(params.frequency_min()) + ":" + QString::number(params.frequency_max())
                  << "-l"
                  << QString::number(params.lna_gain())
                  << "-g"
                  << QString::number(params.vga_gain())
                  << "-w"
                  << QString::number(params.fft_bin_width());

//    [-h] # this help
//    [-d serial_number] # Serial number of desired HackRF
//    [-a amp_enable] # RX RF amplifier 1=Enable, 0=Disable
//    [-f freq_min:freq_max] # minimum and maximum frequencies in MHz
//    [-p antenna_enable] # Antenna port power, 1=Enable, 0=Disable
//    [-l gain_db] # RX LNA (IF) gain, 0-40dB, 8dB steps
//    [-g gain_db] # RX VGA (baseband) gain, 0-62dB, 2dB steps
//    [-n num_samples] # Number of samples per frequency, 16384-4294967296
//    [-w bin_width] # FFT bin width (frequency resolution) in Hz
//    [-1] # one shot mode
//    [-B] # binary output

    return argument_list;
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
        emit signal_output_line(send_data);

        usleep(200);
    }

    if(input_file.isOpen())
        input_file.close();
}

void spectrum_process_worker::save_test_data_to_file(const QString &)
{

}
