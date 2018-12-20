#include "surface_spectr.h"

#include <QtGui/QPainter>
#include <cmath>
#include <QTimer>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

surface_spectr::surface_spectr(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    m_spectr_item_list.clear();
    // level
    m_level_min = -100;
    m_level_max = 0;
    m_level_ticket.insert("0", m_level_max);
    m_level_ticket.insert("-100", m_level_min);

    // frequency Hz
    m_frequency_min = 1000000;
    m_frequency_max = 100000000;
    m_frequency_ticket.insert("1", m_frequency_min);
    m_frequency_ticket.insert("100", m_frequency_max);

    this->setVisible(true);
    this->setFlag(QQuickItem::ItemHasContents);

    m_image_waterfall = QImage(waterfall_size().y(), waterfall_size().y(), QImage::Format_ARGB32);
    m_image_waterfall.fill(QColor(255, 255, 0));
    m_sensitivity_waterfall = 0.05;

    // Generate displayable colors
    QImage img(500, 1, QImage::Format_ARGB32);
    m_colors_waterfall.reserve(img.height());

    QPainter painter;
    painter.begin(&img);
    QLinearGradient gradient;
    gradient.setStart(0, 0);
    gradient.setFinalStop(img.width(), 0);
    gradient.setColorAt(0, QColor(255, 255, 255));
    gradient.setColorAt(0.2, QColor(0, 0, 255));
    gradient.setColorAt(0.4, QColor(255, 240, 0));
    gradient.setColorAt(0.6, QColor(255, 0, 0));
    gradient.setColorAt(1, QColor(0, 0, 0));
    gradient.setSpread(QGradient::PadSpread);
    painter.fillRect(QRect(0, 0, img.width(), 1), QBrush(gradient));
    painter.end();

    for (int i = 0; i < img.width(); i++) {
        QRgb rgb = img.pixel(i, 0);
        m_colors_waterfall.append(rgb);
    }

    // background
    m_color_background = QColor(Qt::darkGray);
    // base surface point
    m_surface_point.setX(50);
    m_surface_point.setY(30);

    connect(this, &QQuickItem::widthChanged,
            this, &surface_spectr::slot_size_changed);
    connect(this, &QQuickItem::heightChanged,
            this, &surface_spectr::slot_size_changed);

    add_spectr_item("spectr_rt");

    update();

    // for test
#ifdef QT_DEBUG
    connect(this, &surface_spectr::signal_power_spectr_test,
            this, &surface_spectr::slot_power_spectr);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_power_spectr_test()));
    timer->start(100);
#endif

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

    if(painter!= Q_NULLPTR)
        painter->drawImage(QRect(waterfall_point().x(), waterfall_point().y(), waterfall_size().x(), waterfall_size().y()), m_image_waterfall, QRect(0, 0, m_image_waterfall.width(), m_image_waterfall.height()));

    // test
//    const auto rect = contentsBoundingRect();
//    const QPolygonF trianglePolygon({{0, 0},
//                                     {rect.width(), 0},
//                                     {0.5 * rect.width(), rect.height()},
//                                     {0,0}});

//    painter->drawPolygon(trianglePolygon);

    QMap<QString, spectr_item*>::const_iterator spectr_it;

    for(spectr_it = m_spectr_item_list.begin(); spectr_it != m_spectr_item_list.end(); spectr_it++)
    {
        for(int j=0; j<spectr_it.value()->raw_data_size()-1; ++j)
        {
            painter->setPen(spectr_it.value()->item_pen());
            painter->drawLine(spectr_it.value()->raw_data_pos(j), spectr_it.value()->raw_data_pos(j+1));
        }
    }

//#ifdef QT_DEBUG
//    qDebug() << Q_FUNC_INFO << "-----------------------------------------";
//    qDebug() << Q_FUNC_INFO << "waterfall_size:" << waterfall_size();
//    qDebug() << Q_FUNC_INFO << "m_surface_point:" << m_surface_point;
//    qDebug() << Q_FUNC_INFO << "level size:" << this->height()/2 - m_surface_point.y();
//    qDebug() << Q_FUNC_INFO << "frequency size:" << this->width() - m_surface_point.x();
//#endif
}

qreal surface_spectr::sensitivity_waterfall() const
{
    return m_sensitivity_waterfall;
}

qreal surface_spectr::split_surface() const
{
    return m_split_surface;
}

double surface_spectr::level_min() const
{
    return m_level_min;
}

double surface_spectr::level_max() const
{
    return m_level_max;
}

void surface_spectr::slot_power_spectr(const QVector<qreal> &spectr)
{
    tmp_f_vector.clear();

    qreal range = 0;
    qreal shift_x = m_surface_point.x()+1;
    qreal shift_y = (this->height()/2 - m_surface_point.y())/100;

    for(int i=0; i<spectr.size(); ++i)
    {
        qreal x = shift_x + range;
        qreal y = (shift_y * spectr.at(i) * -1) + m_surface_point.y();

        QPointF tmp(x, y);
        tmp_f_vector.append(tmp);

        range += (this->width()-m_surface_point.x()*2-1)/spectr.size();
    }

    m_spectr_item_list.value("spectr_rt")->set_raw_data(tmp_f_vector);

    // waterfall
    QImage img(waterfall_size().x(), waterfall_size().y(), QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&img);

    // Draw 1st pixel row: new values
    for (int x = 0; x < img.width(); x++)
    {
        int i1 = static_cast<int>(x * spectr.size() / img.width());
        qreal amplitude = std::log10(std::abs(spectr.at(i1)));
        int value = static_cast<int>(amplitude * static_cast<qreal>(m_sensitivity_waterfall) * static_cast<qreal>(m_colors_waterfall.length()));

        if (value < 0)
            value = 0;
        if (value >= m_colors_waterfall.length())
            value = m_colors_waterfall.length() - 1;

        painter.setPen(m_colors_waterfall.at(value));
        painter.drawRect(x, 0, 1, 5);
    }

    // Draw old values
    if (!m_image_waterfall.isNull()) {
        painter.drawImage(QRect(0, 6, waterfall_size().x(), waterfall_size().y()), m_image_waterfall, QRect(0, 0, m_image_waterfall.width(), m_image_waterfall.height()));
    }

    painter.end();

    // Redraw the item
    m_image_waterfall = img;

    update();
}

void surface_spectr::slot_sensitivity_waterfall(const qreal &value)
{
    m_sensitivity_waterfall = value;
}

void surface_spectr::slot_split_surface(const qreal &value)
{
    m_split_surface = value;
}

void surface_spectr::slot_level_min(const double &value)
{
    m_level_min = value;

    emit signal_level_min_changed();
}

void surface_spectr::slot_level_max(const double &value)
{
    m_level_max = value;

    emit signal_level_max_changed();
}

void surface_spectr::slot_power_spectr_test()
{
    int size = 200;
    QVector<qreal> test_data;
    test_data.reserve(size);

    for(int i=0; i<size; ++i)
    {
        qreal value = rm.generateDouble()*-100;
        test_data.append(value);
    }
    emit signal_power_spectr_test(test_data);
}

void surface_spectr::add_spectr_item(const QString &name)
{
    m_spectr_item_list.insert(name, new spectr_item(name));
}

void surface_spectr::slot_size_changed()
{
    if (!m_image_waterfall.isNull())
    {
        QImage img = QImage(waterfall_size().x(), waterfall_size().y(), QImage::Format_ARGB32);
        img.fill(QColor(255, 255, 255));

        QPainter painter;
        painter.begin(&img);
        painter.drawImage(QRect(waterfall_point().x(), waterfall_point().y(), waterfall_size().x(), waterfall_size().y()), m_image_waterfall, QRect(0, 0, m_image_waterfall.width(), m_image_waterfall.height()));
        painter.end();

        m_image_waterfall = img;

        update();
    }
}

void surface_spectr::level_scale_paint(QPainter *painter)
{
    QPen scale_pen(Qt::black);
    painter->setPen(scale_pen);
    // font
    QFont font = painter->font();
    font.setPointSizeF(10);
    painter->setFont(font);

    QFontMetrics font_metrics(painter->font());


    QLine line(m_surface_point, QPoint(m_surface_point.x(), this->height()/2));
    painter->drawLine(line);

    if(m_level_ticket.keys().size()==2)
    {
        int max_text_height = font_metrics.ascent()/2;

        // min ticket
        QLine min_line(QPoint(m_surface_point.x()-5, this->height()/2),
                       QPoint(m_surface_point.x(), this->height()/2));
        painter->drawLine(min_line);
        const QString min_text = m_level_ticket.key(m_level_min);
        int min_text_width = font_metrics.boundingRect(min_text).width()+5;
        painter->drawText(QPoint(m_surface_point.x()-5-min_text_width, this->height()/2+max_text_height), min_text);

        // max ticket
        QLine max_line(QPoint(m_surface_point.x()-5, m_surface_point.y()), m_surface_point);
        painter->drawLine(max_line);
        const QString max_text = m_level_ticket.key(m_level_max);
        int max_text_width = font_metrics.boundingRect(max_text).width()+5;
        painter->drawText(QPoint(m_surface_point.x()-5-max_text_width, m_surface_point.y()+max_text_height), m_level_ticket.key(m_level_max));
    }
}

void surface_spectr::frequency_scale_paint(QPainter *painter)
{
    QPen scale_pen(Qt::black);
    QLine line_upper(QPoint(m_surface_point.x(), this->height()/2),
                     QPoint(this->width()-m_surface_point.x(), this->height()/2));

    painter->setPen(scale_pen);
    painter->drawLine(line_upper);

    QLine line_lower(QPoint(m_surface_point.x(), this->height()/2+m_surface_point.y()),
                     QPoint(this->width()-m_surface_point.x(), this->height()/2+m_surface_point.y()));

    painter->setPen(scale_pen);
    painter->drawLine(line_lower);
}

void surface_spectr::time_scale_paint(QPainter *painter)
{
    QPen scale_pen(Qt::black);
    QLine line(QPoint(m_surface_point.x(), this->height()/2+m_surface_point.y()),
               QPoint(m_surface_point.x(), this->height()-m_surface_point.y()));

    painter->setPen(scale_pen);
    painter->drawLine(line);
}

QPoint surface_spectr::waterfall_size() const
{
    QPoint size;

    size.setX(static_cast<int>(this->width()-m_surface_point.x()*2-1));
    size.setY(static_cast<int>((this->height()-m_surface_point.y())-(this->height()/2+m_surface_point.y())-2));

    return size;
}

QPoint surface_spectr::waterfall_point() const
{
    QPoint point;

    point.setX(m_surface_point.x()+1);
    point.setY(static_cast<int>(this->height()/2 + m_surface_point.y() + 1));

    return point;
}
