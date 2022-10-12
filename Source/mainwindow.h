#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ScribbleArea;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    ~MainWindow();
    explicit MainWindow(QWidget *parent = 0);

private:
    ScribbleArea *scribbleArea;
};
#endif // MAINWINDOW_H
