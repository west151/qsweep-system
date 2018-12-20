#ifndef SPECTR_ITEM_H
#define SPECTR_ITEM_H

#include <QPen>
#include <QString>

class spectr_item
{
public:
    spectr_item(const QString &name);

    QString name()const;

    void set_item_color(const QColor &color);
    QColor item_color()const;

    void set_item_style(const Qt::PenStyle &style);
    Qt::PenStyle item_style()const;

    void set_raw_data(const QVector<QPointF> &data);

    qint32 raw_data_size()const;
    QPointF raw_data_pos(const qint32 &index)const;

    QPen item_pen()const;

    void clear_data();

private:
    QString m_name;
    QPen m_item_pen;
    QVector<QPointF> m_data;
};

#endif // SPECTR_ITEM_H
