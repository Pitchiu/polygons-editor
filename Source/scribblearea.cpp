#include "scribblearea.h"
#include <QtWidgets>

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    myPenWidth = 1;
    myPenColor = Qt::blue;
    mode = Off;
    activePolygon = NULL;
    activeButton = Create;
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
    if (event->button() == Qt::LeftButton && activeButton==Create)
    {
        // First point hit!
        if(activePolygon!=NULL && GraphicAlgorithms::distance(event->pos(), activePolygon->Points.first()) < MINDIST)
        {
            mode = Off;
            activePolygon->isClosed = true;
            activePolygon->Lines << QLine(activePolygon->Points.last(), activePolygon->Points.first());
            activePolygon = NULL;
            drawImage();
            return;
        }

        for(auto polIt = Polygons.begin(); polIt != Polygons.end(); ++polIt)
            for(auto lineIt = polIt->Lines.begin(); lineIt != polIt->Lines.end(); ++lineIt)
                if(GraphicAlgorithms::distance(event->pos(), *lineIt) < MINDIST) return;

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

    else if(activeButton==Move)
    {
        if(activePolygon != NULL)
        {
            activePolygon = NULL;
            return;
        }
        activePolygon = detectClickedPolygon(event->pos());
        if(activePolygon!=NULL) startPoint = event->pos();
    }
    else if(activeButton==Delete)
    {

    }
}

Polygon* ScribbleArea::detectClickedPolygon(QPoint point)
{
    for(auto polIt = Polygons.begin(); polIt != Polygons.end(); ++polIt)
        for(auto lineIt = polIt->Lines.begin(); lineIt != polIt->Lines.end(); ++lineIt)
            if(GraphicAlgorithms::distance(point, *lineIt) < MINDIST)
                return &*polIt;
    return NULL;
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if(activeButton == Move && activePolygon!=NULL)
    {
        QPoint diff(event->pos().x() - startPoint.x(), event->pos().y() - startPoint.y());
        for(int i=0; i < activePolygon->Lines.count(); i++)
            activePolygon->Lines[i] = QLine(activePolygon->Lines[i].p1()+diff, activePolygon->Lines[i].p2()+diff);
        for(int i=0; i<activePolygon->Points.count(); i++)
            activePolygon->Points[i] += diff;
        startPoint = event->pos();
        drawImage();
    }
    else if (activeButton == Create && mode == Scribbling)
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

void ScribbleArea::drawImage()
{
    image.fill(qRgb(255, 255, 255));
    QPainter painter(&image);
    for(auto polIt = Polygons.begin(); polIt!=Polygons.end(); ++polIt)
    {
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for(auto lineIt = polIt->Lines.begin(); lineIt != polIt->Lines.end(); ++lineIt)
            drawLine(&painter, *lineIt);

        painter.setPen(QPen(myPenColor, MINDIST, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for(auto pointIt = polIt->Points.begin(); pointIt != polIt->Points.end(); ++pointIt)
            drawPoint(&painter, *pointIt);
    }
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if(mode==Scribbling)
    {

        drawLine(&painter, QLine(startPoint, lastPoint));
    }

    update();
}

void ScribbleArea::drawPoint(QPainter *painter, const QPoint &point)
{
    painter->drawPoint(point);
}

void ScribbleArea::drawLine(QPainter *painter, const QLine &l)
{
    painter->drawLine(l.p1(), l.p2());
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
