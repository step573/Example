#include "mainwindow.h"
#include "mypaintwidget.h"
#include "previewwidget.h"

#include <QCoreApplication>
#include <QMainWindow>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QStatusBar>
#include "marchingCubes.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MarchingCubes_UI");

    // Settings file, stylesheet, log, translation....

    // Load styleSheet
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QString test = QDir::currentPath();

    QFile file("../../../styles/marchingcubes_ui.css");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        setStyleSheet(file.readAll());
    }

    mc = new MarchingCubes(this);

    previewWidget = new PreviewWidget(this);
    setCentralWidget(previewWidget);
    connect(previewWidget, &PreviewWidget::calculateMarchingCubes, mc, &MarchingCubes::setPathAndCalculate);

    paintWidget = new MyPaintWidget(this);
    QDockWidget* dockContainer = new QDockWidget("3D View", this);
    dockContainer->setWidget(paintWidget);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dockContainer);
    connect(mc, &MarchingCubes::marchingCubesFinished, paintWidget, &MyPaintWidget::setData);


    setStatusBar(new QStatusBar(this));
}

MainWindow::~MainWindow()
{
}

