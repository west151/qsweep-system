#include "waterfallitem.h"

#include <QtGui/QPainter>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

WaterfallItem::WaterfallItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    connect(this, &QQuickItem::widthChanged,
            this, &WaterfallItem::sizeChanged);
    connect(this, &QQuickItem::heightChanged,
            this, &WaterfallItem::sizeChanged);

    this->setVisible(true);
    this->setFlag(QQuickItem::ItemHasContents);

    _image = QImage((int)this->width(), (int)this->height(), QImage::Format_ARGB32);
    _image.fill(QColor(255, 255, 255));

    // Generate displayable colors
    QImage img(500, 1, QImage::Format_ARGB32);
    _colors.reserve(img.height());
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
        _colors.append(rgb);
    }

    // Repaint this item
    update();
}

void WaterfallItem::paint(QPainter *painter)
{
    painter->drawImage(QRect(0, 0, width(), height()), _image, QRect(0, 0, _image.width(), _image.height()));

//    // test
//    const auto rect = contentsBoundingRect();
//    const QPolygonF trianglePolygon({{0, 0},
//                                     {rect.width(), 0},
//                                     {0.5 * rect.width(), rect.height()},
//                                     {0,0}});

//    painter->drawPolygon(trianglePolygon);
}

void WaterfallItem::onPowerSpectr(const QVector<qreal> &spectr)
{
//#ifdef QT_DEBUG
//    qDebug() << Q_FUNC_INFO << spectr.size();
//#endif

    // Create new image
    QImage img((int)this->width(), (int)this->height(), QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(&img);


    // Draw 1st pixel row: new values
    for (int x = 0; x < img.width(); x++) {
        unsigned i1 = x * spectr.size() / 10 / img.width();
        //float amplitude = std::abs(spectr[i1]);
        float amplitude = qSin(M_PI / 50 * x) + 0.5 + QRandomGenerator::global()->generateDouble();

        //float amplitude = std::log10(std::abs(result[i1]));
        //int value = (int)(amplitude * (float)_sensitivity * (float)_colors.length());
        int value = (int)(amplitude * (float)_colors.length());
        if (value < 0)
            value = 0;
        if (value >= _colors.length())
            value = _colors.length() - 1;

        painter.setPen(_colors[value]);
        painter.drawRect(x, 0, 1, 5);
    }

#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO << img.width() << spectr.size();
#endif

    // Draw old values
    if (!_image.isNull()) {
        painter.drawImage(QRect(0, 6, width(), height()), _image, QRect(0, 0, _image.width(), _image.height()));
    }

    painter.end();

    // Redraw the item
    _image = img;
    update();
}

void WaterfallItem::clear() {
    _image = QImage((int)this->width(), (int)this->height(), QImage::Format_ARGB32);
    _image.fill(QColor(255, 255, 255));
}

void WaterfallItem::sizeChanged()
{
    QImage img = QImage(static_cast<int>(this->width()), static_cast<int>(this->height()), QImage::Format_ARGB32);
    img.fill(QColor(255, 255, 255));

    if (!_image.isNull()) {
        QPainter painter;
        painter.begin(&img);
        painter.drawImage(QRect(0, 0, static_cast<int>(width()), static_cast<int>(height())), _image, QRect(0, 0, _image.width(), _image.height()));
        painter.end();
    }

    _image = img;
    update();
}
