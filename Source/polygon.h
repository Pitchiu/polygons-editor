#ifndef POLYGON_H
#define POLYGON_H
#include <QWidget>

class Polygon
{
public:
    Polygon();
    QVector<QPoint> Points;
    QVector<QLine> Lines;

    friend bool operator ==(const Polygon& p1, const Polygon& p2);
};

bool operator ==(const Polygon& p1, const Polygon& p2);
#endif // POLYGON_H
