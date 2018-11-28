#include "surface_spectr.h"

#include <QtGui/QPainter>
#include <cmath>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

surface_spectr::surface_spectr(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    this->setVisible(true);
    this->setFlag(QQuickItem::ItemHasContents);

    m_sensitivity_waterfall = 0.05;

    update();
}

void surface_spectr::paint(QPainter *painter)
{
    // test
    const auto rect = contentsBoundingRect();
    const QPolygonF trianglePolygon({{0, 0},
                                     {rect.width(), 0},
                                     {0.5 * rect.width(), rect.height()},
                                     {0,0}});

    painter->drawPolygon(trianglePolygon);
}

qreal surface_spectr::sensitivity_waterfall() const
{
    return m_sensitivity_waterfall;
}

qreal surface_spectr::split_surface() const
{
    return m_split_surface;
}

void surface_spectr::slot_power_spectr(const QVector<qreal> &value)
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO << "Size:" << value.size();
#endif
}

void surface_spectr::slot_sensitivity_waterfall(const qreal &value)
{
    m_sensitivity_waterfall = value;
}

void surface_spectr::slot_split_surface(const qreal &value)
{
    m_split_surface = value;
}
