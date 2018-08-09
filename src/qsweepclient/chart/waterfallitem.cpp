#include "waterfallitem.h"

#include <QtGui/QPainter>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

WaterfallItem::WaterfallItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    this->setVisible(true);
    this->setFlag(QQuickItem::ItemHasContents);

//    _image = QImage((int)this->width(), (int)this->height(), QImage::Format_ARGB32);
//    _image.fill(QColor(255, 255, 255));
}

void WaterfallItem::paint(QPainter *painter)
{
    //painter->drawImage(QRect(0, 0, width(), height()), _image, QRect(0, 0, _image.width(), _image.height()));

    // test
    const auto rect = contentsBoundingRect();
    const QPolygonF trianglePolygon({{0, 0},
                                     {rect.width(), 0},
                                     {0.5 * rect.width(), rect.height()},
                                     {0,0}});

    painter->drawPolygon(trianglePolygon);
}

void WaterfallItem::onPowerSpectr(const QVector<qreal> &spectr)
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO << spectr.size();
#endif
}

void WaterfallItem::clear() {
    _image = QImage((int)this->width(), (int)this->height(), QImage::Format_ARGB32);
    _image.fill(QColor(255, 255, 255));
}

//void WaterfallItem::sizeChanged() {
//    QImage img = QImage(static_cast<int>(this->width()), static_cast<int>(this->height()), QImage::Format_ARGB32);
//    img.fill(QColor(255, 255, 255));

//    if (!_image.isNull()) {
//        QPainter painter;
//        painter.begin(&img);
//        painter.drawImage(QRect(0, 0, width(), height()), _image, QRect(0, 0, _image.width(), _image.height()));
//        painter.end();
//    }

//    _image = img;
//    update();
//}
