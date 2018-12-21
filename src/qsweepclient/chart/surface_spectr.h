#ifndef SURFACE_SPECTR_H
#define SURFACE_SPECTR_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QRandomGenerator>

#include "spectr_item.h"

class surface_spectr : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal level_min READ level_min WRITE slot_level_min NOTIFY signal_level_min_changed)
    Q_PROPERTY(qreal level_max READ level_max WRITE slot_level_max NOTIFY signal_level_max_changed)

public:
    explicit surface_spectr(QQuickItem *parent = Q_NULLPTR);
    void paint(QPainter *painter) override;

    qreal sensitivity_waterfall() const;
    qreal split_surface()const;

    // level
    double level_min()const;
    double level_max()const;

public slots:
    void slot_power_spectr(const QVector<qreal> &value);
    void slot_sensitivity_waterfall(const qreal &);
    void slot_split_surface(const qreal &);

    // level
    void slot_level_min(const double &);
    void slot_level_max(const double &);

    // for test
    void slot_power_spectr_test();

    void add_spectr_item(const QString &, const QColor &color);

signals:
    // level
    void signal_level_min_changed();
    void signal_level_max_changed();

    // for test
    void signal_power_spectr_test(const QVector<qreal> &value);

private slots:
    void slot_size_changed();

private:
    qreal m_split_surface;
    QColor m_color_background;

    QPoint m_surface_point;

    QVector<QPointF> spectr_rt_vector;
    QVector<QPointF> spectr_max_vector;

    // level
    double m_level_min;
    double m_level_max;
    QMap<QString, double> m_level_ticket;
    void level_scale_paint(QPainter *painter);

    // frequency
    double m_frequency_min;
    double m_frequency_max;
    QMap<QString, double> m_frequency_ticket;
    void frequency_scale_paint(QPainter *painter);

    // spectr

    // waterfall
    void time_scale_paint(QPainter *painter);
    // size waterfall
    QPoint waterfall_size()const;
    // start point
    QPoint waterfall_point()const;

    QMap<QString, spectr_item*> m_spectr_item_list;

    QImage m_image_waterfall;
    QList<QRgb> m_colors_waterfall;
    qreal m_sensitivity_waterfall;

    // for test
    QRandomGenerator rm;
};

#endif // SURFACE_SPECTR_H
