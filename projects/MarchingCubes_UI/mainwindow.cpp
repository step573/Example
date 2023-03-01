#include "mainwindow.h"
#include "mypaintwidget.h"

#include <QMainWindow>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Example Project");

    paintWidget = new MyPaintWidget(this);
    setCentralWidget(paintWidget);
}

MainWindow::~MainWindow()
{
}

