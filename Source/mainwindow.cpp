#include "mainwindow.h"
#include "scribblearea.h"
#include <QtWidgets>

MainWindow::MainWindow()
{
    scribbleArea = new ScribbleArea;
    setCentralWidget(scribbleArea);
    setWindowTitle(tr("Scribble"));
    resize(500, 500);
}


