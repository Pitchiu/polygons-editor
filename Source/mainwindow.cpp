#include "mainwindow.h"
#include "scribblearea.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)  : QWidget(parent)
{
    setWindowTitle(tr("Polygons editor"));
    ScribbleArea *scribble = new ScribbleArea();
    scribble->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QRadioButton* createButton = new QRadioButton();
    createButton->setText("Create");
    QRadioButton* moveButton = new QRadioButton();
    moveButton->setText("Move");
    QRadioButton* deleteButton = new QRadioButton();
    deleteButton->setText("Delete");

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->setAlignment(Qt::AlignTop);
    vbox->addWidget(createButton);
    vbox->addWidget(moveButton);
    vbox->addWidget(deleteButton);

    createButton->setChecked(true);

    QGroupBox *groupBox = new QGroupBox();
    connect(createButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::Create;});
    connect(moveButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::Move;});
    connect(deleteButton, &QRadioButton::clicked, this, [=](bool checked){if(checked) scribble->activeButton = ScribbleArea::Delete;});

    groupBox->setLayout(vbox);

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(scribble, 0, 0);
    grid->addWidget(groupBox, 0 ,1);
    setLayout(grid);
    resize(1200,600);
}

MainWindow::~MainWindow()
{

}
