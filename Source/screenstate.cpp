#include "screenstate.h"
#include "graphicalgorithms.cpp"
#include <iostream>

using namespace algorithms;


Entity ScreenState::detectClickedEntity(const QPoint &point)
{
    Entity ent;
    QPoint* bestPoint = NULL;
    Polygon* bestPolygon = NULL;

    for(auto polIt = Polygons.begin(); polIt != Polygons.end(); ++polIt)
        for(auto pointIt = polIt->Points.begin(); pointIt != polIt->Points.end(); ++pointIt)
            if(distance(point, *pointIt) < POINTRADIUS && (bestPoint==NULL || distance(point, *pointIt) < distance(point, *bestPoint)))
            {
                bestPoint = &*pointIt;
                bestPolygon = &*polIt;
            }
    if(bestPolygon != NULL)
    {
        ent.polygon = bestPolygon;
        ent.point = bestPoint;
        return ent;
    }

    QLine* bestLine = NULL;

    for(auto polIt = Polygons.begin(); polIt != Polygons.end(); ++polIt)
        for(auto lineIt = polIt->Lines.begin(); lineIt != polIt->Lines.end(); ++lineIt)
            if(distance(point, *lineIt) < LINEBORDER && (bestLine==NULL || distance(point, *lineIt) < distance(point, *bestLine)))
            {
                bestLine = &*lineIt;
                bestPolygon = &*polIt;
            }

    if(bestPolygon != NULL)
    {
        ent.polygon = bestPolygon;
        ent.line = bestLine;
    }
    return ent;
}

ScreenState::ScreenState()
{
    activePolygon = NULL;
    activeLineRelation = NULL;
    activeLine = NULL;
    activePoint = NULL;

    QPoint p1(200,200), p2(200,400), p3(400,200);
    Polygon pol1;
    pol1.Points << p1 << p2 << p3;
    pol1.Lines << QLine(p1,p2) << QLine(p2,p3) << QLine(p3,p1);
    Polygons << pol1;

    QPoint q1(600,400), q2(800,400), q3(600,500);
    Polygon pol2;
    pol2.Points << q1 << q2 << q3;
    pol2.Lines << QLine(q1, q2) << QLine(q2, q3) << QLine(q3, q1);
    Polygons << pol2;

    Relations << std::make_pair(&Polygons[0].Lines[1], &Polygons[1].Lines[1]);
    Lengths << std::make_pair(&Polygons[0].Lines[2], 300);
    fixPolygons();
}

bool ScreenState::handleCreatePolygonClick(const QPoint &clickedPosition)
{
    // First point hit!
    if(activePolygon!=NULL && distance(clickedPosition, activePolygon->Points.first()) < POINTRADIUS)
    {
        settings.activeMode = Off;
        activePolygon->Lines << QLine(activePolygon->Points.last(), activePolygon->Points.first());
        activePolygon = NULL;
        return true;
    }

    for(auto polIt = Polygons.begin(); polIt != Polygons.end(); ++polIt)
        for(auto lineIt = polIt->Lines.begin(); lineIt != polIt->Lines.end(); ++lineIt)
            if(distance(clickedPosition, *lineIt) < LINEBORDER) return false;

    if (settings.activeMode==Off)
    {
        settings.activeMode = On;
        Polygon p;
        p.Points << clickedPosition;
        Polygons << p;
        activePolygon = &Polygons.last();
        lastPoint = clickedPosition;
    }
    else
    {
        activePolygon->Lines << QLine(activePolygon->Points.last(), clickedPosition);
        activePolygon->Points << clickedPosition;
    }
    startPoint = clickedPosition;
    return true;
}

bool ScreenState::handleInsertVertexClick(const QPoint &clickedPosition)
{
    Entity ent = detectClickedEntity(clickedPosition);
    // no polygon detected, or click is too close to existing point
    if(ent.line == NULL)
        return false;

    deleteFromLengths(ent.line);
    deleteFromRelation(ent.line);
    QPointF nearestF = nearestPoint(*ent.line, clickedPosition);
    QPoint nearest = QPoint(nearestF.x(), nearestF.y());

    int lineIndex = ent.polygon->Lines.indexOf(*ent.line);
    // update Points
    ent.polygon->Points.insert(lineIndex+1, nearest);
    // insert
    QPoint oldSecondPoint = ent.line->p2();
    ent.line->setP2(nearest);

    QLine newLine(nearest, oldSecondPoint);

    // insert new line after clicked one
    int clickedIndex = ent.polygon->Lines.indexOf(*ent.line);
    ent.polygon->Lines.insert(clickedIndex+1, newLine);
    return true;
}

bool ScreenState::handleMovePolygonClick(const QPoint &clickedPosition)
{
    if(activePolygon != NULL)
    {
        activePolygon = NULL;
        return false;
    }
    activePolygon = detectClickedEntity(clickedPosition).polygon;
    if(activePolygon!=NULL) startPoint = clickedPosition;
    return false;
}

bool ScreenState::handleMoveLineVertexClick(const QPoint &clickedPosition)
{
    if(settings.activeMode==On)
    {
       settings.activeMode=Off;
       activePoint = NULL;
       activeLine = NULL;
       activePolygon = NULL;
       return false;
    }

    Entity ent = detectClickedEntity(clickedPosition);
    activePolygon = ent.polygon;
    if(activePolygon == NULL)
        return false;

    settings.activeMode=On;

    QPoint* point = ent.point;
    // point clicked
    if(point!=NULL)
    {
        activePoint = point;
        lastPoint = clickedPosition;
        int index = activePolygon->Points.indexOf(*activePoint);
        if(index==0)
        {
            neighbourLines[0] = &activePolygon->Lines[activePolygon->Lines.size()-1];
            neighbourLines[1] = &activePolygon->Lines[0];
        }
        else
        {
            neighbourLines[0] = &activePolygon->Lines[index-1];
            neighbourLines[1] = &activePolygon->Lines[index];
        }
    }
    else
    {
        QLine* line = ent.line;
        activeLine = line;
        lastPoint = clickedPosition;
        int indexClickedLine = activePolygon->Lines.indexOf(*line);
        int linesVectorSize = activePolygon->Lines.size();

        neighbourPoints[0] = &activePolygon->Points[indexClickedLine];
        int indexSecondPoint = indexClickedLine +1 >= linesVectorSize ? 0 : indexClickedLine+1;
        neighbourPoints[1] = &activePolygon->Points[indexSecondPoint];

        int firstLineIndex = indexClickedLine == 0 ? linesVectorSize-1 : indexClickedLine-1;
        int secondLineIndex = indexClickedLine == linesVectorSize-1 ? 0 : indexClickedLine+1;

        neighbourLines[0] = &activePolygon->Lines[firstLineIndex];
        neighbourLines[1] = &activePolygon->Lines[secondLineIndex];
    }
    return false;
}

bool ScreenState::handleSetLengthClick(const QPoint &clickedPosition)
{

    QLine* clickedLine = detectClickedEntity(clickedPosition).line;
    if(clickedLine == NULL) return false;

    QDialog *dialog = new QDialog;
    QLabel *dialogLabel = new QLabel();
    dialogLabel->setText("<b>Give a size</b>");
    dialogLabel->setAlignment(Qt::AlignCenter);
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setValidator(new QIntValidator());
    QPushButton *acceptButton = new QPushButton();
    acceptButton->setText("OK");
    QAbstractButton::connect(acceptButton, &QPushButton::clicked, [=](){
        int result = lineEdit->text().toInt();
        if(result < 0 )
            result = 1;
        if(result>1000) result = 1000;

        bool existAlready = false;
        for(int i=0; i<Lengths.size(); i++)
        {
            if(clickedLine == Lengths[i].first)
            {
                Lengths[i].second = result;
                existAlready = true;
                break;
            }
        }

        if(!existAlready)
            Lengths << std::make_pair(clickedLine, result);

        dialog->accept();
        fixPolygons();
    });

    QGridLayout *dialogLayout = new QGridLayout(dialog);
    dialogLayout->addWidget(dialogLabel, 0, 0);
    dialogLayout->addWidget(lineEdit, 1, 0);
    dialogLayout->addWidget(acceptButton, 2, 0);

    dialog->setModal(true);
    dialog->setLayout(dialogLayout);
    dialog->exec();
    return true;
}

bool ScreenState::handleAddRelationClick(const QPoint &clickedPosition)
{
    QLine* clickedLine = detectClickedEntity(clickedPosition).line;
    if(clickedLine == NULL) return false;

    if(settings.activeMode==Off)
    {
        settings.activeMode = On;
        activeLineRelation = clickedLine;
        return true;
    }
    if(*clickedLine==*activeLineRelation) return false;

    settings.activeMode=Off;
    Relations << std::make_pair(activeLineRelation, clickedLine);
    activeLineRelation = NULL;
    fixPolygons();
    return true;
}

bool ScreenState::handleUnsetLengthClick(const QPoint &clickedPosition)
{
    QLine* line = detectClickedEntity(clickedPosition).line;
    return deleteFromLengths(line);
}

bool ScreenState::deleteFromLengths(QLine *line)
{
    if(line==NULL)
        return false;

    for(int i=0; i<Lengths.size(); i++)
        if(Lengths[i].first == line)
        {
            Lengths.removeAt(i);
            return true;
        }
    return false;
}

bool ScreenState::deleteFromRelation(QLine *line)
{
    if(line==NULL)
        return false;

    bool changed = false;
    for(int i=0; i<Relations.size(); i++)
        if(Relations[i].first == line || Relations[i].second == line)
        {
            Relations.removeAt(i);
            i--;
            changed = true;
        }
    return changed;
}

bool ScreenState::deletePolygonObject(Polygon *p)
{
    for(auto lineIt = p->Lines.begin(); lineIt != p->Lines.end(); ++lineIt)
    {
        deleteFromRelation(&*lineIt);
        deleteFromLengths(&*lineIt);
    }
    Polygons.removeOne(*p);
    return true;
}

bool ScreenState::handleDeleteRelationClick(const QPoint &clickedPosition)
{
    QLine* line = detectClickedEntity(clickedPosition).line;
    return deleteFromRelation(line);
}

bool ScreenState::handleDeleteVertexClick(const QPoint &clickedPosition)
{
    Entity ent = detectClickedEntity(clickedPosition);
    if(ent.point == NULL)
        return false;

    if(ent.polygon->Lines.size()<=3)
        return deletePolygonObject(ent.polygon);

    int indexLineAfter = ent.polygon->Points.indexOf(*ent.point);
    int indexLineBefore = indexLineAfter == 0 ? ent.polygon->Points.size() - 1 : indexLineAfter-1;

    deleteFromRelation(&ent.polygon->Lines[indexLineBefore]);
    deleteFromRelation(&ent.polygon->Lines[indexLineAfter]);
    deleteFromLengths(&ent.polygon->Lines[indexLineBefore]);
    deleteFromLengths(&ent.polygon->Lines[indexLineAfter]);

    ent.polygon->Points.removeOne(*ent.point);
    ent.polygon->Lines[indexLineBefore].setP2(ent.polygon->Lines[indexLineAfter].p2());
    ent.polygon->Lines.removeAt(indexLineAfter);

    fixPolygons();
    return true;
}

bool ScreenState::handleDeletePolygonClick(const QPoint &clickedPosition)
{
    Polygon* p = detectClickedEntity(clickedPosition).polygon;
    if(p==NULL) return false;
    return deletePolygonObject(p);
}


bool ScreenState::handleClickEvent(const QPoint &clickedPosition)
{
    switch(settings.activeButton)
    {
    case createPolygon:
        return handleCreatePolygonClick(clickedPosition);

    case insertVertex:
        return handleInsertVertexClick(clickedPosition);

    case movePolygon:
        return handleMovePolygonClick(clickedPosition);

    case moveLineVertex:
        return handleMoveLineVertexClick(clickedPosition);

    case setLength:
        return handleSetLengthClick(clickedPosition);

    case addRelation:
        return handleAddRelationClick(clickedPosition);

    case unsetLength:
        return handleUnsetLengthClick(clickedPosition);

    case deleteRelation:
        return handleDeleteRelationClick(clickedPosition);

    case deleteVertex:
        return handleDeleteVertexClick(clickedPosition);

    case deletePolygon:
        return handleDeletePolygonClick(clickedPosition);

    default:
        return false;
    }
}

bool ScreenState::handleCreatePolygonMove(const QPoint &newPosition)
{
    if(activePolygon==NULL)
        return false;

    lastPoint = newPosition;
    return true;
}

bool ScreenState::handleMovePolygonMove(const QPoint &newPosition)
{
    if(activePolygon==NULL) return false;

    QPoint diff(newPosition.x() - startPoint.x(), newPosition.y() - startPoint.y());
    for(int i=0; i < activePolygon->Lines.count(); i++)
        activePolygon->Lines[i] = QLine(activePolygon->Lines[i].p1()+diff, activePolygon->Lines[i].p2()+diff);
    for(int i=0; i<activePolygon->Points.count(); i++)
        activePolygon->Points[i] += diff;
    startPoint = newPosition;
    return true;
}

bool ScreenState::handleMoveLineVertexMove(const QPoint &newPosition)
{
    if(settings.activeMode==Off)
        return false;

    QPoint diff = newPosition - lastPoint;

    if(activePoint!=NULL)
    {
        *activePoint+=diff;
    }
    else
    {
        activeLine->setP1(activeLine->p1()+diff);
        activeLine->setP2(activeLine->p2()+diff);
        *neighbourPoints[0] = activeLine->p1();
        *neighbourPoints[1] = activeLine->p2();
    }
    neighbourLines[0]->setP2(neighbourLines[0]->p2()+=diff);
    neighbourLines[1]->setP1(neighbourLines[1]->p1()+=diff);

    lastPoint = newPosition;
    if(activePoint!=NULL)
        fixPolygons(activePoint);
    else
        fixPolygons(neighbourPoints[0], neighbourPoints[1]);
    return true;
}

bool ScreenState::handleMoveEvent(const QPoint &newPosition)
{
    switch(settings.activeButton)
    {
    case createPolygon:
        return handleCreatePolygonMove(newPosition);

    case movePolygon:
        return handleMovePolygonMove(newPosition);

    case moveLineVertex:
        return handleMoveLineVertexMove(newPosition);

    default:
        return false;
    }
}

void ScreenState::moveVertex(Polygon *p, int index, const QPoint &offset)
{

    QPoint newPoint = (p->Points[index] += offset);
    p->Lines[index].setP1(newPoint);
    p->Lines[index==0 ? p->Lines.size()-1 : index - 1].setP2(newPoint);
}

double ScreenState::calculateError()
{
    double sum = 0;
    for(int i=0; i<Relations.size(); i++)
        sum+=abs(algorithms::findAngle(*Relations[i].first, *Relations[i].second))*10000;

    for(int i=0; i<Lengths.size(); i++)
        sum+=abs(distance(Lengths[i].first->p1(), Lengths[i].first->p2()) - Lengths[i].second);
    return sum;
}

void ScreenState::fixPolygons(QPoint* protectedPointA, QPoint* protectedPointB)
{
    int currentError = calculateError();

    bool changed = true;

    QPoint offsets[] = {QPoint(-1,-1), QPoint(-1, 0), QPoint(-1, 1), QPoint(0, -1),
                        QPoint(0, 1), QPoint(1, -1), QPoint(1, 0), QPoint(1, 1)};
    // this is helper to iterate through every point
    QPoint offsetHelpers[] = {QPoint(-1,-1), QPoint(0, 1), QPoint(0, 1), QPoint(1, -2),
                              QPoint(0, 2), QPoint(1, -2), QPoint(0, 1), QPoint(0, 1),
                              QPoint(-1,-1)};

    while(currentError>THRESHOLD && changed == true)
    {
        changed = false;
        for(int i=0; i<Polygons.size(); i++)
            for(int j=0; j<Polygons[i].Points.size(); j++)
            {
                if(protectedPointA != NULL && Polygons[i].Points[j] == *protectedPointA
                        || protectedPointB != NULL && Polygons[i].Points[j] == *protectedPointB)
                    continue;

                int Index = 0;
                double bestError = currentError;
                for(int z = 0; z<8; z++)
                {
                    // move vertex in 8 sides
                    moveVertex(&Polygons[i], j, offsetHelpers[z]);
                    int err = calculateError();
                    if(err < bestError)
                    {
                        Index = z;
                        bestError = err;
                    }
                }
                // move to default position
                moveVertex(&Polygons[i], j, offsetHelpers[8]);
                // move to new position
                if(currentError - bestError > 0.1)
                {
                    moveVertex(&Polygons[i], j, offsets[Index]);
                    currentError = bestError;
                    changed = true;
                }
            }
    }
}
