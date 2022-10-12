#include "graphicalgorithms.h"

qreal GraphicAlgorithms::distance(QPointF a, QPointF b)
{
    return QLineF(a, b).length();
}

qreal GraphicAlgorithms::distance(QPoint a, QLineF b)
{
    qreal dist = distance(a, nearestPoint(b, a));
    return dist;
}

QPointF GraphicAlgorithms::nearestPoint(const QLineF& line, const QPointF& point)
{
    QLineF perpendicLine(point,QPointF(point.x(),0.0));
    perpendicLine.setAngle(90.0+line.angle());
    QPointF result;
    line.intersects(perpendicLine,&result);
    return result;
}


