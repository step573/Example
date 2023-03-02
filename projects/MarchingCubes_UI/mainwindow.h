#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MyPaintWidget;
class PreviewWidget;
class MarchingCubes;

/////////////////////////////////////////////////////////////
///
/// MainWindow
///
/////////////////////////////////////////////////////////////
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MyPaintWidget* paintWidget;
    PreviewWidget* previewWidget;

    // Could be separately inside different thread
    MarchingCubes* mc;
};
#endif // MAINWINDOW_H
