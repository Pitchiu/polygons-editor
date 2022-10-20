#ifndef POLYGON_H
#define POLYGON_H
#include <QWidget>

class Polygon
{
public:
    Polygon();
    QList<QPoint> Points;
    QList<QLine> Lines;
};

#endif // POLYGON_H
