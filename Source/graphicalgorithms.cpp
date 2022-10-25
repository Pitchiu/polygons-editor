#include "graphicalgorithms.h"
#include "screenstate.h"

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
        QLineF perpendicLine(point,QPointF(point.x(),0.0));
        perpendicLine.setAngle(90.0+line.angle());
        QPointF result;
        line.intersects(perpendicLine,&result);

        int left = line.p1().x() < line.p2().x() ? line.p1().x() : line.p2().x();
        int right = (left == line.p1().x()) ? line.p2().x() : line.p1().x();
        int top = line.p1().y() < line.p2().y() ? line.p1().y() : line.p2().y();
        int bottom = (top == line.p1().y()) ? line.p2().y() : line.p1().y();
        if(result.x() < left || result.x() > right || result.y() < top || result.y() > bottom)
            return distance(point, line.p1()) > distance(point, line.p2()) ? line.p2() : line.p1();

        return result;
    }

    double slope(double x1, double y1, double x2, double y2)
    {
        if (x2 - x1 != 0)
            return (y2 - y1) / (x2 - x1);
        return INT_MAX;
    }

    double findAngle(const QLine &L1, const QLine &L2)
    {
        //M = y2 - y1 / x2 - x1
        double M1 = slope(L1.p1().x(), L1.p1().y(), L1.p2().x(), L1.p2().y());
        double M2 = slope(L2.p1().x(), L2.p1().y(), L2.p2().x(), L2.p2().y());
        double angle = abs((M2 - M1) / (1 + M1 * M2));
        double ret = atan(angle);
        double val = (ret * 180) / M_PI;
        return val;
    }
}
