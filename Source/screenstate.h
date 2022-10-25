#ifndef SCREENSTATE_H
#define SCREENSTATE_H
#include "polygon.h"
#include "settings.h"
#include "graphicalgorithms.h"
#include <QtWidgets>
#include <QDialog>
#include <QLabel>

#define POINTRADIUS 10
#define LINETHICKNESS 1
#define LINEBORDER 20
#define THRESHOLD 1

struct Entity
{
    Polygon* polygon = NULL;
    QLine* line = NULL;
    QPoint* point = NULL;
};


class ScreenState
{
public:
    ScreenState();
    bool handleClickEvent(const QPoint &clickedPosition);
    bool handleMoveEvent(const QPoint &movePosition);

    // TODO: move to private and add interface
    Settings settings;
    QPoint startPoint;
    QPoint lastPoint;
    QVector<Polygon> Polygons;
    QVector<std::pair<QLine*, QLine* >> Relations;
    QVector<std::pair<QLine*, double>> Lengths;
    QLine* activeLineRelation;

private:
    Polygon* activePolygon;
    QLine* activeLine;
    QPoint* activePoint;

// this is for moving points and lines
    QLine* neighbourLines[2];
    QPoint* neighbourPoints[2];

    Entity detectClickedEntity(const QPoint &point);

    bool deleteFromRelation(QLine *line);
    bool deleteFromLengths(QLine *line);
    bool deletePolygonObject(Polygon *p);

    void moveVertex(Polygon *p, int index, const QPoint& offset);
    double calculateError();
    void fixPolygons(QPoint* protectedPointA=NULL, QPoint* protectedPointB=NULL);


    // handlers
    bool handleCreatePolygonClick(const QPoint &clickedPosition);
    bool handleInsertVertexClick(const QPoint &clickedPosition);
    bool handleMovePolygonClick(const QPoint &clickedPosition);
    bool handleMoveLineVertexClick(const QPoint &clickedPosition);
    bool handleSetLengthClick(const QPoint &clickedPosition);
    bool handleAddRelationClick(const QPoint &clickedPosition);
    bool handleUnsetLengthClick(const QPoint &clickedPosition);
    bool handleDeleteRelationClick(const QPoint &clickedPosition);
    bool handleDeleteVertexClick(const QPoint &clickedPosition);
    bool handleDeletePolygonClick(const QPoint &clickedPosition);

    bool handleCreatePolygonMove(const QPoint &newPosition);
    bool handleMovePolygonMove(const QPoint &clickedPosition);
    bool handleMoveLineVertexMove(const QPoint &clickedPosition);
};

#endif // SCREENSTATE_H
