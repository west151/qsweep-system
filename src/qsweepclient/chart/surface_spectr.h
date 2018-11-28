#ifndef SURFACE_SPECTR_H
#define SURFACE_SPECTR_H

#include <QQuickPaintedItem>
#include <QImage>

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

private:
    qreal m_split_surface;

    QImage m_image_spectr_rt;
    QList<QRgb> m_colors_spectr_rt;

    QImage m_image_waterfall;
    QList<QRgb> m_colors_waterfall;
    qreal m_sensitivity_waterfall;
};

#endif // SURFACE_SPECTR_H
