#include "datasource.h"
#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtCore/QRandomGenerator>
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

    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();
}

qreal DataSource::minValueY() const
{
    return m_minValueY;
}

qreal DataSource::maxValueY() const
{
    return m_maxValueY;
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
    Q_UNUSED(f_max)

    if(spectr.size()>0)
    {
        m_data.clear();

        QVector<qreal> tmpPower;

        for(qint32 i=0; i<spectr.size(); ++i)
            tmpPower.append(spectr.at(i).m_power);

        QVector<QPointF> points;
        points.reserve(tmpPower.size());

        qreal freq = static_cast<qreal>(f_min);
        qreal step = 0.5;

        for (qint32 j=0; j < tmpPower.size(); ++j) {
            qreal x(0);
            qreal y(0);

            x = freq;
            y = tmpPower.at(j);

            freq = freq + step;

            points.append(QPointF(x, y));
        }
        m_data.append(points);
    }
}
