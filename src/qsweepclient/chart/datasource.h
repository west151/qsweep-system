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

    qreal minValue()const;
    qreal maxValue()const;

public slots:
    void generateData(int type, int rowCount, int colCount);
    void updateDate(const quint64 &, const quint64 &, const QVector<PowerSpectr> &);
    void update(QAbstractSeries *series);

private:
    QList<QVector<QPointF> > m_data;
    int m_index;
    qreal m_minValue;
    qreal m_maxValue;
};

#endif // DATASOURCE_H
