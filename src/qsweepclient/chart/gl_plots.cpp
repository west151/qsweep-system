#include "gl_plots.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

gl_plots::gl_plots(QQuickItem *parent) : QQuickItem(parent),
    m_geometry(QSGGeometry::defaultAttributes_Point2D(), 5),
    m_powerLine(QSGGeometry::defaultAttributes_Point2D(), 2)
{
    setFlag(ItemHasContents);
    m_material.setColor(Qt::black);

    m_materialPowerLine.setColor(Qt::red);
}

void gl_plots::slot_power_spectr(const QVector<qreal> &value)
{
//#ifdef QT_DEBUG
//    qDebug() << Q_FUNC_INFO << "Size:" << value.size();
//#endif
}

QSGNode *gl_plots::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    if(width() <= 0 || height() <= 0)
    {
        delete oldNode;
        return nullptr;
    }

    if (!oldNode)
        oldNode = new QSGNode;

    QSGGeometryNode* geomnode = new QSGGeometryNode();
    QSGGeometry::Point2D* v = m_geometry.vertexDataAsPoint2D();
    const QRectF rect = boundingRect();
    v[0].x = rect.left()+20;
    v[0].y = rect.bottom()-20;

    v[1].x = rect.left()+20; // + rect.width()/3;
    v[1].y = rect.top()+20;

    v[2].x = rect.right()-20;
    v[2].y = rect.top()+20;

    v[3].x = rect.right()-20;
    v[3].y = rect.bottom()-20;

    v[4].x = rect.left()+20;
    v[4].y = rect.bottom()-20;

    geomnode->setGeometry(&m_geometry);
    geomnode->setMaterial(&m_material);

    oldNode->appendChildNode(geomnode);

    //********************************************
    auto geometry1 = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
    geometry1->setLineWidth(2);
    geometry1->setDrawingMode(GL_LINE_STRIP);




//    QSGGeometryNode* geomnode_line = new QSGGeometryNode();
//    QSGGeometry::Point2D *line = m_powerLine.vertexDataAsPoint2D();
//    const QRectF rect_line = boundingRect();

//    line[0].x = rect_line.left()+20;
//    line[0].y = rect_line.bottom()-20;

//    line[1].x = rect_line.right()-20;
//    line[1].y = rect_line.bottom()-20;

//    geomnode_line->setGeometry(&m_powerLine);
//    geomnode_line->setMaterial(&m_materialPowerLine);

//    geomnode->appendChildNode(geomnode_line);

    return oldNode;
}

