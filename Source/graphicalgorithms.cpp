#include "graphicalgorithms.h"

namespace algorithms
{
    qreal distance(QPointF a, QPointF b)
    {
        return QLineF(a, b).length();
    }

    qreal distance(QPoint a, QLineF b)
    {
        qreal dist = distance(a, nearestPoint(b, a));
        return dist;
    }

    QPointF nearestPoint(const QLineF& line, const QPointF& point)
    {
        int left = line.p1().x() < line.p2().x() ? line.p1().x() : line.p2().x();
        int right = (left == line.p1().x()) ? line.p2().x() : line.p1().x();
        int top = line.p1().y() < line.p2().y() ? line.p1().y() : line.p2().y();
        int bottom = (top == line.p1().y()) ? line.p2().y() : line.p1().y();
        if(point.x() < left || point.x() > right || point.y() < top || point.y() > bottom)
            return distance(point, line.p1()) > distance(point, line.p2()) ? line.p2() : line.p1();

        QLineF perpendicLine(point,QPointF(point.x(),0.0));
        perpendicLine.setAngle(90.0+line.angle());
        QPointF result;
        line.intersects(perpendicLine,&result);
        return result;
    }
}
