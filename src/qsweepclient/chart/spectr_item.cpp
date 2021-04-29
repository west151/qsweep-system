#include "spectr_item.h"

spectr_item::spectr_item(const QString &name, const QColor &color)
{
    m_name = name;
    m_item_pen.setColor(color);
}

void spectr_item::set_item_color(const QColor &color)
{
    m_item_pen.setColor(color);
}

QColor spectr_item::item_color() const
{
    return m_item_pen.color();
}

void spectr_item::set_item_style(const Qt::PenStyle &style)
{
    m_item_pen.setStyle(style);
}

Qt::PenStyle spectr_item::item_style() const
{
    return m_item_pen.style();
}

void spectr_item::set_raw_data(const QVector<QPointF> &data)
{
    m_data.clear();
    m_data.append(data);
}

qint32 spectr_item::raw_data_size() const
{
    return m_data.size();
}

QPointF spectr_item::raw_data_pos(const qint32 &index) const
{
    return m_data.at(index);
}

QPen spectr_item::item_pen() const
{
    return m_item_pen;
}

void spectr_item::clear_data()
{
    m_data.clear();
}

QString spectr_item::name() const
{
    return m_name;
}
