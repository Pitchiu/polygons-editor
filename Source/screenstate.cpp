#include "screenstate.h"
#include "graphicalgorithms.cpp"

#define MINDIST 5

using namespace algorithms;

QPoint* ScreenState::detectClickedPoint(const QPoint &point)
{
    for(auto polIt = Polygons.begin(); polIt != Polygons.end(); ++polIt)
        for(auto pointIt = polIt->Points.begin(); pointIt != polIt->Points.end(); ++pointIt)
            if(distance(point, *pointIt) < MINDIST*5)
                return &*pointIt;
    return NULL;
}

QLine* ScreenState::detectClickedLine(const QPoint &point)
{
    for(auto polIt = Polygons.begin(); polIt != Polygons.end(); ++polIt)
        for(auto lineIt = polIt->Lines.begin(); lineIt != polIt->Lines.end(); ++lineIt)
            if(distance(point, *lineIt) < MINDIST*3)
                return &*lineIt;
    return NULL;
}

Polygon* ScreenState::detectClickedPolygon(const QPoint &point)
{
    for(auto polIt = Polygons.begin(); polIt != Polygons.end(); ++polIt)
        for(auto lineIt = polIt->Lines.begin(); lineIt != polIt->Lines.end(); ++lineIt)
            if(distance(point, *lineIt) < MINDIST)
                return &*polIt;
    return NULL;
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
    pol1.Lines << QLine(p1, p2) << QLine(p2, p3) << QLine(p3, p1);
    Polygons << pol1;

    QPoint q1(600,400), q2(800,400), q3(600,500);
    Polygon pol2;
    pol2.Points << q1 << q2 << q3;
    pol2.Lines << QLine(q1, q2) << QLine(q2, q3) << QLine(q3, q1);
    Polygons << pol2;
}

bool ScreenState::handleCreatePolygonClick(const QPoint &clickedPosition)
{
    // First point hit!
    if(activePolygon!=NULL && distance(clickedPosition, activePolygon->Points.first()) < MINDIST)
    {
        settings.activeMode = Off;
        activePolygon->Lines << QLine(activePolygon->Points.last(), activePolygon->Points.first());
        activePolygon = NULL;
        return true;
    }

    for(auto polIt = Polygons.begin(); polIt != Polygons.end(); ++polIt)
        for(auto lineIt = polIt->Lines.begin(); lineIt != polIt->Lines.end(); ++lineIt)
            if(distance(clickedPosition, *lineIt) < MINDIST) return false;

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
    if(detectClickedPoint(clickedPosition) != NULL)
        return false;
    Polygon *polygon = detectClickedPolygon(clickedPosition);

    if(polygon==NULL)
        return false;

    QLine* line = detectClickedLine(clickedPosition);

    if(line==NULL)
        return false;
    deleteFromLengths(line);
    deleteFromRelation(line);
    QPointF nearestF = nearestPoint(*line, clickedPosition);
    QPoint nearest = QPoint(nearestF.x(), nearestF.y());

    // update Points
    polygon->Points.insert(polygon->Points.indexOf(line->p2()), nearest);
    QPoint oldFirstPoint = line->p1();
    line->setP1(nearest);

    // insert new line before old line
    int clickedIndex = polygon->Lines.indexOf(*line);
    polygon->Lines.insert(clickedIndex, QLine(oldFirstPoint, nearest));
    // TODO: run polygon correcting and maybe remove constraints
    return true;
}

bool ScreenState::handleMovePolygonClick(const QPoint &clickedPosition)
{
    if(activePolygon != NULL)
    {
        activePolygon = NULL;
        return false;
    }
    activePolygon = detectClickedPolygon(clickedPosition);
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

    activePolygon = detectClickedPolygon(clickedPosition);
    if(activePolygon == NULL)
        return false;

    QPoint* point = detectClickedPoint(clickedPosition);
    if(point!=NULL)
    {
        settings.activeMode=On;
        activePoint = point;
        lastPoint = clickedPosition;
        // neighbour lines are at index and index-1 positions
        int index = activePolygon->Points.indexOf(*activePoint);
        if(index!=0)
        {
            neighbourLines[0] = &activePolygon->Lines[index-1];
            neighbourLines[1] = &activePolygon->Lines[index];
        }
        else
        {
            neighbourLines[0] = &activePolygon->Lines[activePolygon->Lines.size()-1];
            neighbourLines[1] = &activePolygon->Lines[0];
        }
        return false;
    }

    QLine* line = detectClickedLine(clickedPosition);
    if(line!=NULL)
    {
        settings.activeMode=On;
        activeLine = line;
        lastPoint = clickedPosition;
        int indexp1 = activePolygon->Points.indexOf(activeLine->p1());
        int indexp2 = activePolygon->Points.indexOf(activeLine->p2());
        if(indexp1 < indexp2)
        {
            neighbourPoints[0] = &activePolygon->Points[indexp1];
            neighbourPoints[1] = &activePolygon->Points[indexp2];
        }
        else
        {
            neighbourPoints[0] = &activePolygon->Points[indexp2];
            neighbourPoints[1] = &activePolygon->Points[indexp1];
        }

        int indexClickedLine = activePolygon->Lines.indexOf(*line);
        int linesVectorSize = activePolygon->Lines.size();
        if(indexClickedLine==0)
        {
            neighbourLines[0] = &activePolygon->Lines[linesVectorSize-1];
            neighbourLines[1] = &activePolygon->Lines[1];
        }
        else if(indexClickedLine==linesVectorSize-1)
        {
            neighbourLines[0] = &activePolygon->Lines[linesVectorSize-2];
            neighbourLines[1] = &activePolygon->Lines[0];
        }
        else
        {
            neighbourLines[0] = &activePolygon->Lines[indexClickedLine-1];
            neighbourLines[1] = &activePolygon->Lines[indexClickedLine+1];
        }
    }

    return false;
}

bool ScreenState::handleSetLengthClick(const QPoint &clickedPosition)
{
    QLine* clickedLine = detectClickedLine(clickedPosition);
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
    QLine* clickedLine = detectClickedLine(clickedPosition);
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
    return true;
}

bool ScreenState::handleUnsetLengthClick(const QPoint &clickedPosition)
{
    QLine* line = detectClickedLine(clickedPosition);
    return deleteFromLengths(line);
}

bool ScreenState::deleteFromLengths(QLine *line)
{
    if(line==NULL)
        return false;

    for(int i=0; i<Lengths.size(); i++)
        if(Lengths[i].first==line)
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
    QLine* line = detectClickedLine(clickedPosition);
    return deleteFromRelation(line);
}

bool ScreenState::handleDeleteVertexClick(const QPoint &clickedPosition)
{
    QPoint* point = detectClickedPoint(clickedPosition);
    if(point == NULL)
        return false;

    Polygon* polygon = detectClickedPolygon(clickedPosition);
    if(polygon==NULL)
        return false;

    if(polygon->Lines.size()<=3)
        return deletePolygonObject(polygon);

    QLine* l1 ,*l2;
    int i = 0;
    for(; i<polygon->Lines.size(); i++)
    {
        if(polygon->Lines[i].p2() == *point)
        {
            l1 = &polygon->Lines[i];
            if(i==polygon->Lines.size()-1)
                i=0;
            else
                i++;

            l2 = &polygon->Lines[i];
            break;
        }
    }

    if(!l1 || !l2 )
        return false;
    QLine newLine(l1->p1(), l2->p2());
    polygon->Lines.insert(i, newLine);

    polygon->Points.removeOne(*point);
    polygon->Lines.removeOne(*l1);
    polygon->Lines.removeOne(*l2);
    return true;
}

bool ScreenState::handleDeletePolygonClick(const QPoint &clickedPosition)
{
    Polygon* p = detectClickedPolygon(clickedPosition);
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
    // TODO
    //Opcje: przesuniety wierzchołek, lub dwa wierzchołki. One są nietykalne. Żaden wierzchołek -np. dodanie relacji
    //przesunięty wierzchołek - poprawiamy dwa wierzchołki koło niego we wszystkie strony po raz.
    //sprawdzamy za każdym razem wskaźnik zepsucia dla całej sceny.
    //jesli nic sie nie poprawilo, nie przeusuwam i usuwam wierzcholek z listy do obrobienia
    //jesli sie poprawilo, wybieram to ustawienie gdzie się poprawiło najbardziej i jeśli sąsiednie boki
    //są w jakiś relacjach to wywołuje dla nich poprawianie
    //jeśli
    // dwa wskaźniki: wskaźnik zepsucia kąta, wskaźnik zepsucia długości.
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
