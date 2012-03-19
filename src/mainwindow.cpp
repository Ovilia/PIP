#include <QFileDialog>
#include <QPixmap>

#include "HistogramDialog.h"
#include "ImagePolicy.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabWidget(0),
    originWidget(0),
    binaryWidget(0),
    histogramDialog(0),
    imageProcessor(0),
    isFirstImage(true)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete originWidget;
    delete binaryWidget;
    delete tabWidget;
    delete histogramDialog;
    delete imageProcessor;
}

void MainWindow::repaintBinary()
{
    QImage* binaryImage = imageProcessor->getBinaryImage();
    binaryWidget->setPixmap(QPixmap::fromImage(*binaryImage));
    binaryWidget->repaint();
}

void MainWindow::on_actionOpen_triggered()
{
    imagePath = QFileDialog::getOpenFileName(
                this, tr("Open an Image"), QDir::currentPath(),
                tr("Image files (*bmp *jpeg *jpg *.png);;All files (*.*)"));

    // load the image and update UI
    if (!imagePath.isNull()) {
        // set imageProcessor
        if (imageProcessor) {
            imageProcessor->setImage(imagePath);
        } else {
            imageProcessor = new ImageProcessor(imagePath);
        }

        if (isFirstImage) {
            tabWidget = new QTabWidget(this);
            tabWidget->setTabsClosable(true);
        } else {
            // remove previous tabs
            int amt = tabWidget->count();
            for (int i = 0; i < amt; ++i) {
                tabWidget->removeTab(0);
            }
        }

        // add a tab containing the image
        QPixmap pixmap(imagePath);
        originWidget = new ImageWidget(pixmap, tabWidget);
        tabWidget->addTab(originWidget, "Origin Image");

        if (isFirstImage) {
            // add tabWidget only if it is the first image opened
            ui->gridLayout->addWidget(tabWidget);
            // enable UI components
            ui->actionHistogram->setEnabled(true);
        } else {
            // reset UI components
            ui->actionHistogram->setChecked(false);
            binaryWidget = 0;
        }

        // isFirstImage is set to be false after first opening
        isFirstImage = false;
    }
}

void MainWindow::on_actionHistogram_triggered()
{
    QImage* binaryImage = imageProcessor->getBinaryImage();
    binaryWidget = new ImageWidget(
                QPixmap::fromImage(*binaryImage), tabWidget);
    tabWidget->addTab(binaryWidget, "Binary Image");
    tabWidget->setCurrentWidget(binaryWidget);

    histogramDialog = new HistogramDialog(this, imageProcessor);
    histogramDialog->show();
}
