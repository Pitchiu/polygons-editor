#ifndef SETTINGS_H
#define SETTINGS_H

enum LineAlgorithm
{
    defaultAlgorithm,
    BresenhamAlgorithm
};

enum Button
{
    createPolygon,
    insertVertex,
    movePolygon,
    moveLineVertex,
    setLength,
    addRelation,
    unsetLength,
    deleteRelation,
    deleteVertex,
    deletePolygon
};

enum ActiveMode
{
    Off,
    On
};

class Settings
{
public:
    Settings();
    LineAlgorithm activeAlgorithm;
    Button activeButton;
    ActiveMode activeMode;
};


#endif // SETTINGS_H
