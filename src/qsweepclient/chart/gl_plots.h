#ifndef GL_PLOTS_H
#define GL_PLOTS_H

#include <QQuickItem>
#include <QSGGeometryNode>

// surface

class gl_plots : public QQuickItem
{
    Q_OBJECT
public:
    explicit gl_plots(QQuickItem *parent = nullptr);

signals:

public slots:

protected:
    virtual QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

private:
    QSGGeometryNode* m_border_active_node;
};

#endif // GL_PLOTS_H
