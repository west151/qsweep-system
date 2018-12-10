#include "surface_spectr.h"

#include <QtGui/QPainter>
#include <cmath>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

surface_spectr::surface_spectr(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    m_spectr_item_list.clear();
//    // level
//    m_level_min = -100;
//    m_level_max = 0;
//    // frequency
//    m_frequency_min = 1;
//    m_frequency_max = 100;

    this->setVisible(true);
    this->setFlag(QQuickItem::ItemHasContents);

    m_sensitivity_waterfall = 0.05;

    // background
    m_color_background = QColor(Qt::darkGray);

    connect(this, &QQuickItem::widthChanged,
            this, &surface_spectr::slot_size_changed);
    connect(this, &QQuickItem::heightChanged,
            this, &surface_spectr::slot_size_changed);

    update();
}

void surface_spectr::paint(QPainter *painter)
{
    QBrush brush(m_color_background);

    painter->setBrush(brush);
    painter->drawRect(QRectF(0,0,this->width(),this->height()));

    // level scale
    level_scale_paint(painter);
    // frequency scale
    frequency_scale_paint(painter);
    // waterfall
    time_scale_paint(painter);


//    if(painter!= Q_NULLPTR)
//        painter->drawImage(QRect(10, 10, static_cast<int>(width()), static_cast<int>(height())), m_image_background, QRect(0, 0, m_image_background.width(), m_image_background.height()));

    // test
//    const auto rect = contentsBoundingRect();
//    const QPolygonF trianglePolygon({{0, 0},
//                                     {rect.width(), 0},
//                                     {0.5 * rect.width(), rect.height()},
//                                     {0,0}});

//    painter->drawPolygon(trianglePolygon);
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

void surface_spectr::slot_init()
{
//    // background
//    if(!m_image_background.isNull())
//    {
//        m_image_background = QImage(static_cast<int>(this->width()), static_cast<int>(this->height()), QImage::Format_ARGB32);
//        m_image_background.fill(m_color_background);
//    }
}

void surface_spectr::slot_size_changed()
{
//    if (!m_image_background.isNull())
//    {
//        QImage img = QImage(static_cast<int>(this->width()), static_cast<int>(this->height()), QImage::Format_ARGB32);
//        img.fill(m_color_background);

//        QPainter painter;
//        painter.begin(&img);
//        painter.drawImage(QRect(0, 0, static_cast<int>(width()), static_cast<int>(height())), m_image_background, QRect(0, 0, m_image_background.width(), m_image_background.height()));
//        painter.end();

//        m_image_background = img;

//        update();

//#ifdef QT_DEBUG
//    qDebug() << Q_FUNC_INFO;
//#endif
    //    }
}

void surface_spectr::level_scale_paint(QPainter *painter)
{
    QPen scalePen(Qt::black);
    QLine line(QPoint(30,30), QPoint(30,this->height()/2));

    painter->setPen(scalePen);
    painter->drawLine(line);
}

void surface_spectr::frequency_scale_paint(QPainter *painter)
{
    QPen scale_pen(Qt::black);
    QLine line_upper(QPoint(30, this->height()/2), QPoint(this->width()-30, this->height()/2));

    painter->setPen(scale_pen);
    painter->drawLine(line_upper);

    QLine line_lower(QPoint(30, this->height()/2+30), QPoint(this->width()-30, this->height()/2+30));

    painter->setPen(scale_pen);
    painter->drawLine(line_lower);
}

void surface_spectr::time_scale_paint(QPainter *painter)
{
    QPen scalePen(Qt::black);
    QLine line(QPoint(30, this->height()/2+30), QPoint(30, this->height()-30));

    painter->setPen(scalePen);
    painter->drawLine(line);
}
