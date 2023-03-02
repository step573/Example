#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>

class QListView;
class QStringListModel;
class QPushButton;
class QLabel;
class MarchingCubes;
class QToolButton;
class QSlider;

/////////////////////////////////////////////////////////////
///
/// PreviewWidget - image loader and preview
///
/// TODO: better threshold selector
///
/////////////////////////////////////////////////////////////
class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);

signals:
    void calculateMarchingCubes(const QStringList& list, int threshold);

private slots:
    void openFileClicked();
    void calculateClicked();

private:
    void clearContent();

private:
    QListView* listView;
    QStringListModel* listModel;
    QPushButton* openFileButton;
    QToolButton* calculateButton;
    QLabel* imageLabel;
    QSlider* thresholdSlider;
};

#endif // PREVIEWWIDGET_H
