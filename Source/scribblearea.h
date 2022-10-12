#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QList>

#include "graphicalgorithms.h"

#define MINDIST 10

class Polygon;

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:

    ScribbleArea(QWidget *parent = 0);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }
    enum Button
    {
        Create,
        Move,
        Delete
    };
    Button activeButton;

public slots:
    void clearImage();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void drawPoint(QPainter *painter, const QPoint &point);
    void drawLine(QPainter *painter, const QLine &l);
    void drawImage();
    void resizeImage(QImage *image, const QSize &newSize);
    Polygon* detectClickedPolygon(QPoint point);

    bool scribbling;
    int myPenWidth;

    QColor myPenColor;
    QImage image;
    QPoint startPoint;
    QPoint lastPoint;
    Polygon* activePolygon;

    QList<Polygon> Polygons;

    enum Mode
    {
        Off,
        Scribbling
    };
    Mode mode;    
};

class Polygon
{
public:
    bool isClosed = false;
    QList<QPoint> Points;
    QList<QLine> Lines;
};


#endif // SCRIBBLEAREA_H


