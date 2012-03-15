#include <QFileDialog>
#include <QPixmap>

#include "ImageStrategy.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isFirstImage = true;
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
        // init imageProcessor
        imageProcessor = new ImageProcessor(imagePath);

        if (isFirstImage) {
            tabWidget = new QTabWidget(this);
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
        }
        // enable UI components
        ui->actionGray_Scale->setEnabled(true);
        ui->actionHistogram->setEnabled(true);

        // isFirstImage is set to be false after first opening
        isFirstImage = false;
    }
}

void MainWindow::on_actionGray_Scale_triggered()
{
    QImage* grayScaleImage = imageProcessor->getGrayScaleImage(
                ImageStrategy::MATCH_LUMINANCE);
    QPixmap pixmap = QPixmap::fromImage(*grayScaleImage);
    grayScaleWidget = new ImageWidget(pixmap, tabWidget);
    tabWidget->addTab(grayScaleWidget, "Gray Scale Image");
    tabWidget->setCurrentWidget(grayScaleWidget);
}
