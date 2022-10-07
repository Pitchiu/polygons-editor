#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class ScribbleArea : public QWidget
{
    // Declares our class as a QObject which is the base class
    // for all Qt objects
    // QObjects handle events
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);

    // Has the image been modified since last save
    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;

    // Updates the scribble area where we are painting
    //void paintEvent(QPaintEvent *event) override;

    // Makes sure the area we are drawing on remains
    // as large as the widget
    //void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void drawPoint(const QPoint &endPoint);
    // Will be marked true or false depending on if
    // we have saved after a change
    bool modified;

    // Marked true or false depending on if the user
    // is drawing
    bool scribbling;

    // Holds the current pen width & color
    int myPenWidth;

    QColor myPenColor;

    // Stores the image being drawn
    QImage image;

    // Stores the location at the current mouse event
    QPoint lastPoint;
};
#endif // SCRIBBLEAREA_H


