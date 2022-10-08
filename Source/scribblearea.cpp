#include "scribblearea.h"
#include <QtWidgets>

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    // Roots the widget to the top left even if resized
    setAttribute(Qt::WA_StaticContents);
    setMouseTracking(true);
    myPenWidth = 5;
    myPenColor = Qt::blue;
    mode = Off;
    activePolygon = NULL;
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(activePolygon!=NULL && distance(event->pos(), activePolygon->Points.first()) < MINDIST)
        {
            mode = Off;
            activePolygon->isClosed = true;
            activePolygon->Lines << QLine(activePolygon->Points.last(), activePolygon->Points.first());
            activePolygon = NULL;
            return;
        }

        for(auto polIt = Polygons.begin(); polIt != Polygons.end(); ++polIt)
            for(auto lineIt = polIt->Lines.begin(); lineIt != polIt->Lines.end(); ++lineIt)
                if(distance(event->pos(), *lineIt) < MINDIST) return;

        if (mode==Off)
        {
            mode = Scribbling;
        }
        else if(activePolygon == NULL)
        {
            Polygon p;
            p.Points << startPoint << event->pos();
            p.Lines << QLine(startPoint,event->pos());
            Polygons << p;
            activePolygon = &Polygons.last();
        }
        else if(event->pos() != activePolygon->Points.first())
        {
            activePolygon->Lines << QLine(activePolygon->Points.last(), event->pos());
            activePolygon->Points << event->pos();
        }
        startPoint = event->pos();
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if (mode == Scribbling)
    {
        lastPoint = event->pos();
        drawImage();
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{

}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::drawPoint(const QPoint &endPoint)
{

}

void ScribbleArea::drawImage()
{
    image.fill(qRgb(255, 255, 255));
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    for(auto it = Polygons.begin(); it!=Polygons.end(); ++it)
    painter.drawLines(it->Lines);
    if(mode==Scribbling)
        painter.drawLine(startPoint, lastPoint);
    update();
}

void ScribbleArea::drawLine(const QPoint &startPoint, const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

//this functions should be moved to some helper class
qreal ScribbleArea::distance(QPointF a, QPointF b)
{
    return QLineF(a, b).length();
}

QPointF nearestPoint(const QLineF& line, const QPointF& point);

//QPointF nearestPoint(const QLineF& line, const QPointF& point);
qreal ScribbleArea::distance(QPoint a, QLineF b)
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
return result;
}
