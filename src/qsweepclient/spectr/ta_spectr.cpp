#include "ta_spectr.h"
#include "data_spectr.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

ta_spectr::ta_spectr(QObject *parent) : QObject(parent)
{
}

void ta_spectr::slot_data_spectr(const data_spectr &data)
{
    QVector<power_spectr> tmp_spectr(data.spectr());

    std::sort(tmp_spectr.begin(), tmp_spectr.end(), [](const power_spectr& a, const power_spectr& b) {
        return a.hz_low < b.hz_low;
    });

    if(tmp_spectr.size() > 0)
    {
        QVector<qreal> tmp_power_rt;

        for(qint32 i=0; i<tmp_spectr.size(); ++i)
            tmp_power_rt.append(tmp_spectr.at(i).m_power);

        const auto dt = tmp_spectr.at(0).m_date_time;

        emit signal_spectr_rt(dt, tmp_spectr.at(0).hz_low, tmp_spectr.at(tmp_spectr.size()-1).hz_high, tmp_power_rt);

#ifdef QT_DEBUG
        qDebug() << "if(tmp_spectr.size()>0)";
#endif
    }
}
