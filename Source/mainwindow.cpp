#include "mainwindow.h"
#include "scribblearea.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)  : QWidget(parent)
{
    setWindowTitle(tr("Polygons editor"));
    ScribbleArea *scribble = new ScribbleArea();
    scribble->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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

    connect(createPolygonButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::createPolygon;});
    connect(insertVertexButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::insertVertex;});
    connect(movePolygonButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::movePolygon;});
    connect(moveLineVertexButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::moveLineVertex;});
    connect(setLengthButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::setLength;});
    connect(addRelationButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::addRelation;});
    connect(unsetLengthButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::unsetLength;});
    connect(deleteRelationButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::deleteRelation;});
    connect(deleteVertexButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::deleteVertex;});
    connect(deletePolygonButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::deletePolygon;});

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

    connect(defaultAlgorithmButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeAlgorithm = ScribbleArea::defaultAlgorithm;});
    connect(bresenhamAlgorithmButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeAlgorithm = ScribbleArea::BresenhamAlgorithm;});

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

    grid->addWidget(scribble, 0 ,0);
    grid->addWidget(rightBox, 0, 1);
    setLayout(grid);
    resize(1200,600);
}

MainWindow::~MainWindow()
{

}
