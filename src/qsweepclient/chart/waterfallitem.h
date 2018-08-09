#ifndef WATERFALLITEM_H
#define WATERFALLITEM_H

#include <QQuickPaintedItem>
#include <QImage>

class WaterfallItem : public QQuickPaintedItem
{
    Q_OBJECT
    //Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )

public:
    explicit WaterfallItem(QQuickItem *parent = Q_NULLPTR);
    void paint(QPainter *painter) override;

    void clear();

public slots:
    void onPowerSpectr(const QVector<qreal> &);
//    void samplesCollected(std::vector<float> *samples);
    //void sizeChanged();

private:
    QImage _image;
    QList<QRgb> _colors;
};

#endif // WATERFALLITEM_H
