#include "scribblearea.h"
#include <QtWidgets>

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
}

void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;

    if(screenstate.handleClickEvent(event->pos()))
        drawImage();
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if(screenstate.handleMoveEvent(event->pos()))
        drawImage();
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
    painter.setPen(QPen(Qt::black, LINETHICKNESS, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if(screenstate.settings.activeButton==createPolygon && screenstate.settings.activeMode==On)
    {
        drawLine(&painter, QLine(screenstate.startPoint, screenstate.lastPoint));
    }

    for(auto polIt = screenstate.Polygons.begin(); polIt!=screenstate.Polygons.end(); ++polIt)
    {
        painter.setPen(QPen(Qt::black, LINETHICKNESS, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for(auto lineIt = polIt->Lines.begin(); lineIt != polIt->Lines.end(); ++lineIt)
            drawLine(&painter, *lineIt);

        painter.setPen(QPen(Qt::black, POINTRADIUS, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for(auto pointIt = polIt->Points.begin(); pointIt != polIt->Points.end(); ++pointIt)
            painter.drawPoint(*pointIt);
    }
    if(screenstate.activeLineRelation!=NULL)
    {
        painter.setPen(QPen(Qt::red, LINETHICKNESS, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        drawLine(&painter, *screenstate.activeLineRelation);
    }
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QFont font = painter.font() ;
    font.setPointSize(font.pointSize() + 10);
    painter.setFont(font);

    // drawing lengths
    for(int i=0; i<screenstate.Lengths.size(); i++)
        painter.drawText(screenstate.Lengths[i].first->center(),
            QString(std::to_string((int)screenstate.Lengths[i].second).c_str()));

    // drawing relations
    for(int i=0; i<screenstate.Relations.size(); i++)
    {
        painter.drawText(screenstate.Relations[i].first->center(),
            QString(("||" + std::to_string(i+1)).c_str()));

        painter.drawText(screenstate.Relations[i].second->center(),
            QString(("||" + std::to_string(i+1)).c_str()));
    }
    update();
}

void ScribbleArea::drawLine(QPainter *painter, const QLine &l)
{
    if(screenstate.settings.activeAlgorithm==defaultAlgorithm)
    {
        painter->drawLine(l.p1(), l.p2());
        return;
    }

    int x = l.p1().x();
    int x2 = l.p2().x();
    int y = l.p1().y();
    int y2 = l.p2().y();

    int w = x2 - x;
    int h = y2 - y ;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;
    if (w<0) dx1 = -1 ; else if (w>0) dx1 = 1 ;
    if (h<0) dy1 = -1 ; else if (h>0) dy1 = 1 ;
    if (w<0) dx2 = -1 ; else if (w>0) dx2 = 1 ;
    int longest = abs(w);
    int shortest = abs(h) ;
    if (!(longest>shortest)) {
        longest = abs(h) ;
        shortest = abs(w) ;
        if (h<0) dy2 = -1 ; else if (h>0) dy2 = 1 ;
        dx2 = 0 ;
    }
    int numerator = longest >> 1 ;
    for (int i=0;i<=longest;i++) {
        painter->drawPoint(x,y);
        numerator += shortest ;
        if (!(numerator<longest)) {
            numerator -= longest ;
            x += dx1 ;
            y += dy1 ;
        } else {
            x += dx2 ;
            y += dy2 ;
        }
    }
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
