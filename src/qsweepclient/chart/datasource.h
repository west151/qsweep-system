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

public slots:
    void updateDate(const quint32 &, const quint32 &, const QVector<PowerSpectr> &);
    void update(QAbstractSeries *series);

private:
    QList<QVector<QPointF> > m_data;
    int m_index;
    qreal m_minValueY;
    qreal m_maxValueY;
};

#endif // DATASOURCE_H
