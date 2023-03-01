#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MyPaintWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MyPaintWidget* paintWidget;

};
#endif // MAINWINDOW_H
