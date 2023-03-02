#include "previewwidget.h"
#include <QFileIconProvider>
#include <QBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QMenu>
#include <QWidgetAction>
#include <QLabel>
#include <QStringListModel>
#include <QListView>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QSlider>

PreviewWidget::PreviewWidget(QWidget *parent) : QWidget(parent)
{
    listModel = new QStringListModel(this);
    listView = new QListView(this);
    listView->setModel(listModel);
    connect(listView, &QListView::clicked, this, [=](const QModelIndex& index) {
        QString path = index.data().toString();
        if (QFile::exists(path)){
            imageLabel->setPixmap(QPixmap(path));
        }

    });

    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(true);
    imageLabel->setObjectName("PreviewWidgetImage");

    QFileIconProvider icons;
    openFileButton = new QPushButton(this);
    openFileButton->setObjectName("PreviewWidgetOpenButton");
    openFileButton->setIcon(icons.icon(QFileIconProvider::Folder));
    openFileButton->setStatusTip(tr("Open Image Files"));
    openFileButton->setToolTip(tr("Open Image Files"));
    connect(openFileButton, &QPushButton::clicked, this, &PreviewWidget::openFileClicked);

    calculateButton = new QToolButton(this);
    calculateButton->setPopupMode(QToolButton::ToolButtonPopupMode::MenuButtonPopup);
    calculateButton->setObjectName("PreviewWidgetCalcButton");
    calculateButton->setStatusTip(tr("Calculate MC"));
    calculateButton->setToolTip(tr("Calculate MC"));
    connect(calculateButton, &QPushButton::clicked, this, &PreviewWidget::calculateClicked);

    thresholdSlider = new QSlider(Qt::Horizontal, this);
    thresholdSlider->setMaximum(255);
    thresholdSlider->setMinimum(0);
    thresholdSlider->setValue(120);
    QMenu* menu = new QMenu(calculateButton);
    QWidgetAction* sliderContainer = new QWidgetAction(menu);
    sliderContainer->setDefaultWidget(thresholdSlider);
    menu->addAction(sliderContainer);
    calculateButton->setMenu(menu);

    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(openFileButton);
    buttonLayout->addWidget(calculateButton);
    buttonLayout->addStretch();

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(listView, 5);
    mainLayout->addWidget(imageLabel, 1);
    mainLayout->addStretch(1);
    mainLayout->addLayout(buttonLayout, 2);
    setLayout(mainLayout);

    clearContent();
}

void PreviewWidget::openFileClicked()
{
    QStringList fileName = QFileDialog::getOpenFileNames(this,
                                                         "Open image file", "",
                                                         "Images (*.png *.jpg)", 0, QFileDialog::DontUseNativeDialog);
    if (fileName.isEmpty()) {
        clearContent();
    } else{
        listModel->setStringList(fileName);
        calculateButton->setEnabled(true);
    }
}

void PreviewWidget::calculateClicked()
{
    emit calculateMarchingCubes(listModel->stringList(), thresholdSlider->value());
}

void PreviewWidget::clearContent()
{
    listModel->setStringList({tr("No Files Loaded")});
    imageLabel->clear();
    imageLabel->setText(tr("Image \nPreview"));
    calculateButton->setEnabled(false);
}
