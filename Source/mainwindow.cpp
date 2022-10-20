#include "mainwindow.h"
#include "scribblearea.h"
#include <QtWidgets>
#include <QScreen>


MainWindow::MainWindow(QWidget *parent)  : QWidget(parent)
{
    setWindowTitle(tr("Polygons editor"));

    scribbleArea.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QRadioButton* createPolygonButton = new QRadioButton();
    createPolygonButton->setText("Create polygon");
    QRadioButton* insertVertexButton = new QRadioButton();
    insertVertexButton->setText("Insert vertex");
    QRadioButton* movePolygonButton = new QRadioButton();
    movePolygonButton->setText("Move polygon");
    QRadioButton* moveLineVertexButton = new QRadioButton();
    moveLineVertexButton->setText("Move line/vertex");
    QRadioButton* setLengthButton = new QRadioButton();
    setLengthButton->setText("Set length");
    QRadioButton* addRelationButton = new QRadioButton();
    addRelationButton->setText("Add relation");
    QRadioButton* unsetLengthButton = new QRadioButton();
    unsetLengthButton->setText("Unset Length");
    QRadioButton* deleteRelationButton = new QRadioButton();
    deleteRelationButton->setText("Delete relation");
    QRadioButton* deleteVertexButton = new QRadioButton();
    deleteVertexButton->setText("Delete vertex");
    QRadioButton* deletePolygonButton = new QRadioButton();
    deletePolygonButton->setText("Delete polygon");

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->setAlignment(Qt::AlignTop);

    vbox->addWidget(createPolygonButton);
    vbox->addWidget(insertVertexButton);
    vbox->addWidget(movePolygonButton);
    vbox->addWidget(moveLineVertexButton);
    vbox->addWidget(setLengthButton);
    vbox->addWidget(addRelationButton);
    vbox->addWidget(unsetLengthButton);
    vbox->addWidget(deleteRelationButton);
    vbox->addWidget(deleteVertexButton);
    vbox->addWidget(deletePolygonButton);

    createPolygonButton->setChecked(true);

    connect(createPolygonButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeButton = createPolygon;});
    connect(insertVertexButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeButton = insertVertex;});
    connect(movePolygonButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeButton = movePolygon;});
    connect(moveLineVertexButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeButton = moveLineVertex;});
    connect(setLengthButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeButton = setLength;});
    connect(addRelationButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeButton = addRelation;});
    connect(unsetLengthButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeButton = unsetLength;});
    connect(deleteRelationButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeButton = deleteRelation;});
    connect(deleteVertexButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeButton = deleteVertex;});
    connect(deletePolygonButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeButton = deletePolygon;});

    QGroupBox *groupBox = new QGroupBox();
    groupBox->setLayout(vbox);


    QRadioButton* defaultAlgorithmButton = new QRadioButton();
    defaultAlgorithmButton->setText("Default algorithm");
    QRadioButton* bresenhamAlgorithmButton = new QRadioButton();
    bresenhamAlgorithmButton->setText("Bresenham algorithm");

    QVBoxLayout* layoutAlgorithm = new QVBoxLayout;
    layoutAlgorithm->setAlignment(Qt::AlignTop);

    layoutAlgorithm->addWidget(defaultAlgorithmButton);
    layoutAlgorithm->addWidget(bresenhamAlgorithmButton);

    connect(defaultAlgorithmButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeAlgorithm = defaultAlgorithm;});
    connect(bresenhamAlgorithmButton, &QRadioButton::clicked, this, [&](bool checked){if(checked) scribbleArea.screenstate.settings.activeAlgorithm = BresenhamAlgorithm;});

    defaultAlgorithmButton->setChecked(true);

    QGroupBox *algorithmBox = new QGroupBox();
    algorithmBox->setLayout(layoutAlgorithm);

    QGridLayout *grid = new QGridLayout(this);

    QGroupBox *rightBox = new QGroupBox();
    QGridLayout *rightGrid = new QGridLayout();

    QLabel *optionsLabel = new QLabel();
    optionsLabel->setText("<b>Options</b>");
    optionsLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    optionsLabel->setAlignment(Qt::AlignCenter);

    QLabel *algorithmLabel = new QLabel();
    algorithmLabel->setText("<b>Draw algorithm</b>");
    algorithmLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    algorithmLabel->setAlignment(Qt::AlignCenter);

    groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    algorithmBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    rightGrid->addWidget(optionsLabel, 0, 0);
    rightGrid->addWidget(groupBox, 1, 0);
    rightGrid->addWidget(algorithmLabel, 2, 0);
    rightGrid->addWidget(algorithmBox, 3, 0);

    rightBox->setLayout(rightGrid);

    grid->addWidget(&scribbleArea, 0 ,0);
    grid->addWidget(rightBox, 0, 1);
    setLayout(grid);
    resize(1200,600);

}

MainWindow::~MainWindow()
{

}
