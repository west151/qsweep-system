#ifndef GL_PLOTS_H
#define GL_PLOTS_H

#include <QQuickItem>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

// surface

class gl_plots : public QQuickItem
{
    Q_OBJECT
public:
    explicit gl_plots(QQuickItem *parent = nullptr);

signals:

public slots:
    void slot_power_spectr(const QVector<qreal> &value);

protected:
    virtual QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

private:
    QSGGeometry m_geometry;
    QSGFlatColorMaterial m_material;

    QSGGeometry m_powerLine;
    QSGFlatColorMaterial m_materialPowerLine;
};

#endif // GL_PLOTS_H
