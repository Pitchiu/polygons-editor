#include "QWidget"

namespace algorithms
{
    static qreal distance(QPointF a, QPointF b);
    static qreal distance(QPoint a, QLineF b);
    static QPointF nearestPoint(const QLineF& line, const QPointF& point);
    static double findAngle(const QLine &L1, const QLine &L2);
    static double slope(double x1, double y1, double x2, double y2);
};

