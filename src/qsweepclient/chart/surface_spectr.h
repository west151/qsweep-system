#ifndef SURFACE_SPECTR_H
#define SURFACE_SPECTR_H

#include <QQuickPaintedItem>
#include <QImage>

#include "spectr_item.h"

class surface_spectr : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit surface_spectr(QQuickItem *parent = Q_NULLPTR);
    void paint(QPainter *painter) override;

    qreal sensitivity_waterfall() const;
    qreal split_surface()const;

public slots:
    void slot_power_spectr(const QVector<qreal> &value);
    void slot_sensitivity_waterfall(const qreal &);
    void slot_split_surface(const qreal &);
    void slot_init();

private slots:
    void slot_size_changed();

private:
    qreal m_split_surface;
    QColor m_color_background;

    // level
    double m_level_min;
    double m_level_max;
    void level_scale_paint(QPainter *painter);

    // frequency
    double m_frequency_min;
    double m_frequency_max;
    void frequency_scale_paint(QPainter *painter);

    // waterfall
    void time_scale_paint(QPainter *painter);

    QMap<QString, spectr_item*> m_spectr_item_list;

    QImage m_image_waterfall;
    QList<QRgb> m_colors_waterfall;
    qreal m_sensitivity_waterfall;

    // for test
    QVector<QPointF> m_test_data;
};

#endif // SURFACE_SPECTR_H
