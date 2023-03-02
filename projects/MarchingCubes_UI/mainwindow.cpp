#include "mainwindow.h"
#include <QCoreApplication>
#include <QMainWindow>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include "mypaintwidget.h"
#include "previewwidget.h"
#include "marchingCubes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MarchingCubes_UI");

    // Settings file, stylesheet, log, translation....

    // Load styleSheet
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QFile file("styles/marchingcubes_ui.css");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        setStyleSheet(file.readAll());
    }

    // MC
    mc = new MarchingCubes(this);

    // Central and dock widgets
    previewWidget = new PreviewWidget(this);
    connect(previewWidget, &PreviewWidget::calculateMarchingCubes, mc, &MarchingCubes::setPathAndCalculate);
    setCentralWidget(previewWidget);

    paintWidget = new MyPaintWidget(this);
    connect(mc, &MarchingCubes::marchingCubesFinished, paintWidget, &MyPaintWidget::setData);
    QDockWidget* dockContainer = new QDockWidget(tr("3D View"), this);
    dockContainer->setWidget(paintWidget);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dockContainer);

    setStatusBar(new QStatusBar(this));

    QToolBar* toolBar = new QToolBar(this);
    QAction* actionAboutApp = new QAction(tr("About App"), toolBar);
    connect(actionAboutApp, &QAction::triggered, this, [=](bool checked){
        QMessageBox::about(this, tr("About App"), "Application for calculating MC \nAuthor: Stepan Brazdil  ");
    });
    QAction* actionAboutQt = new QAction(tr("About Qt"), toolBar);
    connect(actionAboutQt, &QAction::triggered, this, [=](bool checked){
        QMessageBox::aboutQt(this, tr("About Qt"));
    });
    toolBar->addAction(actionAboutApp);
    toolBar->addAction(actionAboutQt);
    addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);
}

MainWindow::~MainWindow()
{
}

