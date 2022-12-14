#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QList>

#include "graphicalgorithms.h"
#include "screenstate.h"

class Polygon;

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:

    ScribbleArea(QWidget *parent = 0);
    void clearImage();
    ScreenState screenstate;
    void drawImage();


private:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void drawLine(QPainter *painter, const QLine &l);
    void resizeImage(QImage *image, const QSize &newSize);

    QImage image;
};

#endif // SCRIBBLEAREA_H


