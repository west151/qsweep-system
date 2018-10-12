#include "datasource.h"
#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtCore/QtMath>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(QObject *parent) : QObject(parent)
{
    m_minValueY = -100;
    m_maxValueY = 0;

    isMaxPowerSpectr = true;
    m_countAvg = 0;
    isClearPowerSpectr = false;

    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    m_minFreq = 0;
    m_maxFreq = 0;
    m_spectSize = 0;
    m_step = 0;
}

qreal DataSource::minValueY() const
{
    return m_minValueY;
}

qreal DataSource::maxValueY() const
{
    return m_maxValueY;
}

void DataSource::setCountAvg(const qint32 &value)
{
    m_countAvg = value;
}

void DataSource::setMaxPowerSpectr(const bool &on)
{
    isMaxPowerSpectr = on;
}

void DataSource::clearMaxPowerSpectr()
{
    isClearPowerSpectr = true;
}

void DataSource::update(QAbstractSeries *series)
{
    if(m_data.size()>0){
        if (series) {
            QXYSeries *xySeries = static_cast<QXYSeries *>(series);
            m_index++;
            if (m_index > m_data.count() - 1)
                m_index = 0;

            QVector<QPointF> points = m_data.at(m_index);
            // Use replace instead of clear + append, it's optimized for performance
            xySeries->replace(points);
        }
    }
}

void DataSource::updateDate(const quint32 &f_min, const quint32 &f_max, const QVector<PowerSpectr> &spectr)
{
    if(spectr.size()>0)
    {
        QVector<qreal> tmpPower;

        for(qint32 i=0; i<spectr.size(); ++i)
            tmpPower.append(spectr.at(i).m_power);

        emit sendPowerSpectr(tmpPower);

        // real time
        QVector<QPointF> pointsRT;
        pointsRT.reserve(tmpPower.size());
        // max
        QVector<QPointF> pointsMAX;
        pointsMAX.reserve(tmpPower.size());

        qreal minFreq = static_cast<qreal>(f_min);
        qreal maxFreq = static_cast<qreal>(f_max);
        qreal step = (maxFreq - minFreq)/tmpPower.size();

        bool isLastPower(false);
        if(m_lastPower.size() == tmpPower.size())
            isLastPower = true;

        for (qint32 j=0; j < tmpPower.size(); ++j) {
            qreal x(0);
            qreal y(0);

            x = minFreq;
            y = tmpPower.at(j);

            minFreq = minFreq + step;

            pointsRT.append(QPointF(x, y));

            if(isLastPower)
            {
                qreal y1 = qMax(m_lastPower.at(j), y);
                m_lastPower[j] = y1;
                pointsMAX.append(QPointF(x, y1));
            }
        }
        m_data.clear();
        m_data.append(pointsRT);
        m_data.append(pointsMAX);

        // last power
        if((m_lastPower.size() != tmpPower.size())||isClearPowerSpectr){
            m_lastPower.clear();
            m_lastPower.append(tmpPower);
            isClearPowerSpectr = false;
        }
    }
}
