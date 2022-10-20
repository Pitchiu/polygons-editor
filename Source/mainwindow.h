#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scribblearea.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    ~MainWindow();
    explicit MainWindow(QWidget *parent = 0);
    ScribbleArea scribbleArea;
};
#endif // MAINWINDOW_H
