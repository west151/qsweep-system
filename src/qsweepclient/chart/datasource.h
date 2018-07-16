#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QtCharts/QAbstractSeries>

QT_CHARTS_USE_NAMESPACE

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QObject *parent = nullptr);

signals:

public slots:
    void generateData(int type, int rowCount, int colCount);
    void update(QAbstractSeries *series);

private:
    QList<QVector<QPointF> > m_data;
    int m_index;
};

#endif // DATASOURCE_H
