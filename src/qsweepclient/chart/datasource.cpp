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
    m_minValue = -50;
    m_maxValue = 50;

    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    //generateData(0, 10, 1024);
}

qreal DataSource::minValue() const
{
    return m_minValue;
}

qreal DataSource::maxValue() const
{
    return m_maxValue;
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

void DataSource::generateData(int type, int rowCount, int colCount)
{
    // Remove previous data
    m_data.clear();

    // Append the new data depending on the type
    for (int i(0); i < rowCount; i++) {
        QVector<QPointF> points;
        points.reserve(colCount);
        for (int j(0); j < colCount; j++) {
            qreal x(0);
            qreal y(0);
            switch (type) {
            case 0:
                // data with sin + random component
                y = qSin(M_PI / 50 * j) + 0.5 + QRandomGenerator::global()->generateDouble();
                x = j;
                break;
            case 1:
                // linear data
                x = j;
                y = static_cast<qreal>(i/10);
                break;
            default:
                // unknown, do nothing
                break;
            }
            points.append(QPointF(x, y));
        }
        m_data.append(points);
    }
}

void DataSource::updateDate(const quint64 &f_min, const quint64 &f_max, const QVector<PowerSpectr> &spectr)
{
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

        for (qint32 j(0); j < tmpPower.size(); ++j) {
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
