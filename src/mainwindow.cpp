#include <QFileDialog>
#include <QPixmap>

#include "HistogramDialog.h"
#include "ImageStrategy.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabWidget(0),
    originWidget(0),
    grayScaleWidget(0),
    imageProcessor(0),
    isFirstImage(true)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete originWidget;
    delete grayScaleWidget;
    delete tabWidget;
    delete imageProcessor;
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
            ui->actionGray_Scale->setEnabled(true);
            ui->actionHistogram->setEnabled(true);
        } else {
            // reset UI components
            ui->actionGray_Scale->setChecked(false);
            ui->actionHistogram->setChecked(false);
            grayScaleWidget = 0;
        }

        // isFirstImage is set to be false after first opening
        isFirstImage = false;
    }
}

void MainWindow::on_actionGray_Scale_triggered()
{
    if (ui->actionGray_Scale->isChecked()) {
        if (!grayScaleWidget) {
            // calculate it if not exists
            QImage* grayScaleImage = imageProcessor->getGrayScaleImage(
                        ImageStrategy::MATCH_LUMINANCE);
            QPixmap pixmap = QPixmap::fromImage(*grayScaleImage);
            grayScaleWidget = new ImageWidget(pixmap, tabWidget);
        }
        tabWidget->addTab(grayScaleWidget, "Gray Scale Image");
        tabWidget->setCurrentWidget(grayScaleWidget);
    } else {
        tabWidget->removeTab(tabWidget->indexOf(grayScaleWidget));
    }
}

void MainWindow::on_actionHistogram_triggered()
{
}
