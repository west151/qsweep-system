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

    void clear();

    // level
    double level_min()const;
    double level_max()const;

public slots:
    void slot_power_spectr(const QVector<qreal> &spectr);
    void slot_sensitivity_waterfall(const qreal &);
    void slot_split_surface(const qreal &);

    // view max spectr
    void slot_spectr_max_calc(const bool &);

    // level
    void slot_level_min(const double &);
    void slot_level_max(const double &);

    // for test
    void slot_power_spectr_test();

    void add_spectr_item(const QString &, const QColor &color);
    void remove_spectr_item(const QString &);

signals:
    // level
    void signal_level_min_changed();
    void signal_level_max_changed();

    // for test
    void signal_power_spectr_test(const QVector<qreal> &value);

private slots:
    void slot_size_changed();

private:
    bool is_spectr_max_calc;

    qreal m_split_surface;
    QColor m_color_background;
    QColor m_color_axis;

    QPoint m_surface_point;

    QVector<QPointF> spectr_rt_vector;
    QVector<qreal> spectr_max_value;
    QVector<QPointF> spectr_max_vector;

    // level
    double m_level_min;
    double m_level_max;
    qint32 m_ticket_segment;

    // frequency
    double m_frequency_min;
    double m_frequency_max;

    // spectr
    QPoint spectr_size()const;  // size spectr
    void spectr_surface_paint(QPainter *painter);

    // waterfall
    QPoint waterfall_size()const;   // size waterfall
    QPoint waterfall_point()const;  // start point
    void waterfall_surface_paint(QPainter *painter);

    QMap<QString, spectr_item*> m_spectr_item_list;

    QImage m_image_waterfall;
    QList<QRgb> m_colors_waterfall;
    qreal m_sensitivity_waterfall;

    // for test
    QRandomGenerator rm;
};

#endif // SURFACE_SPECTR_H
