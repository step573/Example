#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>

class QListView;
class QStringListModel;
class QPushButton;
class QLabel;
class MarchingCubes;

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);

signals:
    void calculateMarchingCubes(const QStringList& list);

private slots:
    void openFileClicked();
    void calculateClicked();

private:
    void clearContent();

private:
    QListView* listView;
    QStringListModel* listModel;
    QPushButton* openFileButton;
    QPushButton* calculateButton;
    QLabel* imageLabel;


};

#endif // PREVIEWWIDGET_H
