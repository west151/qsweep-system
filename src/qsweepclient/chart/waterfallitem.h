#ifndef WATERFALLITEM_H
#define WATERFALLITEM_H

#include <QQuickPaintedItem>
#include <QImage>

class WaterfallItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal sensitivity READ sensitivity WRITE setSensitivity NOTIFY sensitivityChanged)

public:
    explicit WaterfallItem(QQuickItem *parent = Q_NULLPTR);
    void paint(QPainter *painter) override;

    void clear();

public slots:
    void onPowerSpectr(const QVector<qreal> &);
    void setSensitivity(const qreal &);
    qreal sensitivity() const;

signals:
    void sensitivityChanged();


private slots:
    void sizeChanged();

private:
    QImage _image;
    QList<QRgb> _colors;
    qreal _sensitivity;
};

#endif // WATERFALLITEM_H
