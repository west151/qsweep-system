#include "gl_plots.h"

gl_plots::gl_plots(QQuickItem *parent) : QQuickItem(parent)
{

}

QSGNode *gl_plots::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{

    //    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);
    //        if (!node) {
    //            node = new QSGGeometryNode;

    //            QSGVertexColorMaterial * material = new QSGVertexColorMaterial;
    //            node->setMaterial(material);
    //            node->setFlag(QSGNode::OwnsMaterial);
    //        }

    //        // if GetMyGeometry returns every time a new dynamically allocated object then you should
    //        // call node->setFlag(QSGNode::OwnsGeometry) to not leak memory here:
    //        QSGGeometry * geometry = GetMyGeometry();
    //        node->setGeometry(geometry);
    //        // No need to call node->markDirty(QSGNode::DirtyGeometry) because setGeometry is called.

    //        return node;

//==============================================================================

//    if (!oldNode) {
//        oldNode = new QSGNode;
//      }

//      oldNode->removeAllChildNodes();

//      QSGGeometry * geometry = GetMyGeometry();
//      QSGVertexColorMaterial * material = new QSGVertexColorMaterial;
//      QSGGeometryNode * child_node = new QSGGeometryNode;

//      child_node->setGeometry(geometry);
//      child_node->setMaterial(material);
//      child_node->setFlag(QSGNode::OwnsMaterial);
//      oldNode->appendChildNode(child_node);

//      return oldNode;

// =============================================================================

//    Q_UNUSED(updatePaintNodeData)

//    // Если при обновлении нода не существует, то необходимо создать все объекты и прикрепить их к ноде
//    if (!oldNode)
//    {
//        // Функция для отрисовки круга
//        auto drawCircle = [this](double radius, QSGGeometry* geometry)
//        {
//            for (int i = 0; i < 360; ++i)
//            {
//                double rad = (i - 90) * Deg2Rad;
//                geometry->vertexDataAsPoint2D()[i].set(std::cos(rad) * radius + width() / 2, std::sin(rad) * radius + height() / 2);
//            }
//        };

//        // Создаём внешний неактивный обод по 360 точкам с помощью геометрии вершин
//        QSGGeometry* borderNonActiveGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 360);
//        borderNonActiveGeometry->setDrawingMode(GL_POLYGON); // Отрисовка будет в виде полигона, то есть с заливкой
//        drawCircle(width() / 2, borderNonActiveGeometry); // Установка координат всех точек обода

//        // Цвет неактивной части обода
//        QSGFlatColorMaterial* borderNonActiveMaterial = new QSGFlatColorMaterial();
//        borderNonActiveMaterial->setColor(m_borderNonActiveColor);

//        // Создаём ноду для отрисовки через геометрию вершин
//        QSGGeometryNode* borderNonActiveNode = new QSGGeometryNode();
//        borderNonActiveNode->setGeometry(borderNonActiveGeometry); // Установка геометрии
//        borderNonActiveNode->setMaterial(borderNonActiveMaterial); // Установка материала
//        // Устанавливаем ноду в качестве парента для геометрии и материала,
//        // Чтобы при уничтожении нода очистила память от этих объектов
//        borderNonActiveNode->setFlags(QSGNode::OwnsGeometry|QSGNode::OwnsMaterial);

//        //-----------------------------------------------------------------------------------------------
//        // Создание объекта для отрисовки активной части обода, по началу не используем ни одной точки
//        QSGGeometry* borderActiveGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
//        borderActiveGeometry->setDrawingMode(GL_POLYGON); // Также отрисовка будет в качестве полигона

//        // Цвет активной части обода
//        QSGFlatColorMaterial* borderActiveMaterial = new QSGFlatColorMaterial();
//        borderActiveMaterial->setColor(m_borderActiveColor);

//        // Нам потребуется указатель на эту ноду, поскольку именно её геометрию придётся постоянно менять
//        m_borderActiveNode = new QSGGeometryNode();
//        m_borderActiveNode->setGeometry(borderActiveGeometry); // Установка геометрии
//        m_borderActiveNode->setMaterial(borderActiveMaterial); // Установка материала
//        // Устанавливаем ноду в качестве парента для геометрии и материала,
//        // Чтобы при уничтожении нода очистила память от этих объектов
//        m_borderActiveNode->setFlags(QSGNode::OwnsGeometry|QSGNode::OwnsMaterial);
//        // Прикрепляем ноду к родительской
//        borderNonActiveNode->appendChildNode(m_borderActiveNode);

//        //-----------------------------------------------------------------------------------------------
//        // Создание внутреннего фона таймера, также по 360 точкам
//        QSGGeometry* backgroundGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 360);
//        backgroundGeometry->setDrawingMode(GL_POLYGON); // Отрисовываем как полигон
//        drawCircle(width() / 2 - 10, backgroundGeometry); // Установка координат всех точек внутреннего фона

//        // Цвет внутреннего фона
//        QSGFlatColorMaterial* backgroundMaterial = new QSGFlatColorMaterial();
//        backgroundMaterial->setColor(m_backgroundColor);

//        // Создаём ноду фона
//        QSGGeometryNode* backgroundNode = new QSGGeometryNode();
//        backgroundNode->setGeometry(backgroundGeometry); // Установка геометрии
//        backgroundNode->setMaterial(backgroundMaterial); // Установка материала
//        // Устанавливаем ноду в качестве парента для геометрии и материала,
//        // Чтобы при уничтожении нода очистила память от этих объектов
//        backgroundNode->setFlags(QSGNode::OwnsGeometry|QSGNode::OwnsMaterial);
//        // Прикрепляем ноду к родительской
//        borderNonActiveNode->appendChildNode(backgroundNode);

//        // Возвращаем все отрисованные ноды в первоначальном состоянии
//        return borderNonActiveNode;
//    }
//    else
//    {
//        // Если родительская нода существует, значит всё инициализовано и можно отрисовывать активный обод
//        static const double radius = width() / 2;
//        // Получаем количество точек
//        int countPoints = static_cast<int>(angle());
//        // Берём геометрию из ноды активной части обода
//        QSGGeometry* geometry = m_borderActiveNode->geometry();
//        // Перевыделяем память под точки
//        geometry->allocate(countPoints + 1, 0);
//        // Уведомляем все отрисовщики об изменении геометрии ноды
//        m_borderActiveNode->markDirty(QSGNode::DirtyGeometry);
//        // отрисовываем центральную точку
//        geometry->vertexDataAsPoint2D()[0].set(radius, radius);

//        // А также все остальные точки
//        for (int i = 1; i < countPoints + 1; ++i)
//        {
//            double rad = (i - 90) * Deg2Rad;
//            geometry->vertexDataAsPoint2D()[i].set(std::cos(rad) * radius + width() / 2, std::sin(rad) * radius + height() / 2);
//        }
//    }

//    // Если нода существовала, то возвращаем старую ноду
//    return oldNode;
}

