#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QList>

#define MINDIST 10

class Polygon;

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

public slots:
    void clearImage();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void drawPoint(const QPoint &endPoint);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    enum Mode
    {
        Off,
        Scribbling
    };

private:
    void drawLine(const QPoint &startPoint, const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void drawImage();

    bool modified;
    bool scribbling;
    int myPenWidth;

    //this should be moved
    qreal distance(QPointF a, QPointF b);
    qreal distance(QPoint a, QLineF b);

    QColor myPenColor;
    QImage image;
    QPoint startPoint;
    QPoint lastPoint;
    Polygon* activePolygon;

    QList<Polygon> Polygons;
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


