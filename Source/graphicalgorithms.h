#include "QWidget"

class GraphicAlgorithms
{
public:
    static qreal distance(QPointF a, QPointF b);
    static qreal distance(QPoint a, QLineF b);
    static QPointF nearestPoint(const QLineF& line, const QPointF& point);
};

