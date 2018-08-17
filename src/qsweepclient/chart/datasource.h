#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QtCharts/QAbstractSeries>
#include "qsweepspectr.h"

QT_CHARTS_USE_NAMESPACE

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QObject *parent = nullptr);

    qreal minValueY()const;
    qreal maxValueY()const;

    void setCountAvg(const qint32 &);
    void setMaxPowerSpectr(const bool &);

signals:
    // send spectr to Waterfall
    void sendPowerSpectr(const QVector<qreal> &);

public slots:
    void updateDate(const quint32 &, const quint32 &, const QVector<PowerSpectr> &);
    void update(QAbstractSeries *series);       // realtime
    void clearMaxPowerSpectr();

private:
    // realtime
    QList<QVector<QPointF> > m_data;
    QVector<qreal> m_lastPower;
    int m_index;
    qreal m_minValueY;
    qreal m_maxValueY;
    qint32 m_countAvg;
    bool isMaxPowerSpectr;
    bool isClearPowerSpectr;

    quint32 m_minFreq;
    quint32 m_maxFreq;
    qint32 m_spectSize;
    qreal m_step;
};

#endif // DATASOURCE_H
