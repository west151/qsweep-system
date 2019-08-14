#include "parser_worker.h"
#include "sweep_message.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

// hackrf_sweep -f 2300:2700 -l 32 -g 20 -w 125000 -r 2300_2700_125000.out
// hackrf_sweep -f 2300:2700 -l 32 -g 20 -w 100000 -r 2300_2700_100000.out

//Output fields:
//	date, time, hz_low, hz_high, hz_bin_width, num_samples, dB, dB, . . .

parser_worker::parser_worker(QObject *parent) : QObject(parent),
    is_parser_range(false),
    is_complete_parser_range(false)
{

}

void parser_worker::slot_input_line(const QByteArray &line)
{
    const auto list_ba_data(line.split(','));

    if(list_ba_data.size()>0)
    {
        quint64 hz_low = list_ba_data.at(2).toULong();
        quint64 hz_high = list_ba_data.at(3).toULong();

        if(hz_low == 2300000000) {
            is_parser_range = true;
            is_complete_parser_range = false;
            buffer_power_db.clear();

#ifdef QT_DEBUG
            qDebug() << "start parsing data:" << hz_low;
#endif
        }

        if(hz_high == 2700000000) {
            is_complete_parser_range = true;
        }

        if(is_parser_range)
        {
            power_spectr tmp_power_spectr;

            int num_samples = list_ba_data.at(5).toInt();

            tmp_power_spectr.hz_low = hz_low;
            tmp_power_spectr.hz_high = hz_high;
            tmp_power_spectr.m_fft_bin_width = list_ba_data.at(4).toDouble();

            for(int i=6; i<list_ba_data.size(); ++i)
                tmp_power_spectr.m_power.append(list_ba_data.at(i).toDouble());

            buffer_power_db.append(tmp_power_spectr);

            //#ifdef QT_DEBUG
            //        qDebug() << list_ba_data.at(0)
            //                 << list_ba_data.at(1)
            //                 << hz_low
            //                 << hz_high
            //                 << QString::number(tmp_power_spectr.m_fft_bin_width, 'f', 2)
            //                 << num_samples
            //                 << tmp_power_spectr.m_power.size();
            //#endif
            if(is_complete_parser_range)
            {
                sweep_message send_data;
                send_data.set_type(type_message::data_spectr);

                data_spectr spectr;
                spectr.set_id_params(QString("fdsatbvf"));
                spectr.set_spectr(buffer_power_db);

                send_data.set_data_message(spectr.to_json());

                emit signal_data_spectr_message(send_data.export_json());

                is_complete_parser_range = false;
                buffer_power_db.clear();

#ifdef QT_DEBUG
                qDebug() << "sending data to remote host:" << hz_high;
                qDebug() << "----------------------------------------";
#endif
            }
        }
    }
}
