#ifndef SCREENSTATE_H
#define SCREENSTATE_H
#include "polygon.h"
#include "settings.h"
#include "graphicalgorithms.h"
#include <QtWidgets>
#include <QDialog>
#include <QLabel>


class ScreenState
{
public:
    ScreenState();
    void fixPolygons();
    bool handleClickEvent(const QPoint &clickedPosition);
    bool handleMoveEvent(const QPoint &movePosition);

    // TODO: move to private and add interface
    Settings settings;
    QPoint startPoint;
    QPoint lastPoint;
    QList<Polygon> Polygons;
    QLine* activeLineRelation;

private:
    QList<std::pair<QLine*, QLine* >> Relations;
    QList<std::pair<QLine*, double>> Lengths;

    Polygon* activePolygon;

    Polygon* detectClickedPolygon(const QPoint &point);
    QLine* detectClickedLine(const QPoint &point);

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
