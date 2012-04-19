#define TEAM_WORK

#include <QFileDialog>
#include <QPixmap>

#include "HistogramDialog.h"
#include "ImagePolicy.h"
#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabWidget(0),
    originWidget(0),
    grayWidget(0),
    binaryWidget(0),
    filteredWidget(0),
    scaledWidget(0),
    rotatedWidget(0),
    algebraWidget(0),
    equalWidget(0),
    contrastWidget(0),
    brightWidget(0),
    histogramDialog(0),
    filterDialog(0),
    scaledDialog(0),
    rotatedDialog(0),
    algebraDialog(0),
    contrastDialog(0),
    brightDialog(0),
    imageProcessor(0),
    isFirstImage(true)
{
    ui->setupUi(this);

#ifndef TEAM_WORK
    ui->actionBrightness->setVisible(false);
    ui->actionContrast->setVisible(false);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
    if (originWidget) {
        delete originWidget;
    }
    if (grayWidget) {
        delete grayWidget;
    }
    if (binaryWidget) {
        delete binaryWidget;
    }
    if (filteredWidget) {
        delete filteredWidget;
    }
    if (scaledWidget) {
        delete scaledWidget->getImage();
        delete scaledWidget;
    }
    if (rotatedWidget) {
        delete rotatedWidget->getImage();
        delete rotatedWidget;
    }
    if (algebraWidget) {
        delete algebraWidget->getImage();
        delete algebraWidget;
    }
    if (equalWidget) {
        delete equalWidget;
    }
    if (contrastWidget) {
        delete contrastWidget;
    }
    if (brightWidget) {
        delete brightWidget;
    }
    if (tabWidget) {
        delete tabWidget;
    }
    if (histogramDialog) {
        delete histogramDialog;
    }
    if (filterDialog) {
        delete filterDialog;
    }
    if (algebraDialog) {
        delete algebraDialog;
    }
    if (contrastDialog) {
        delete contrastDialog;
    }
    if (brightDialog) {
        delete brightDialog;
    }
    if (imageProcessor) {
        delete imageProcessor;
    }
}

ImageProcessor* MainWindow::getImageProcessor()
{
    return imageProcessor;
}

void MainWindow::repaintBinary()
{
    binaryWidget->setImage(imageProcessor->getBinaryImage());
    binaryWidget->repaint();
}

void MainWindow::setFilteredImage(QImage *image)
{
    if (!filteredWidget) {
        filteredWidget = new ImageWidget(image);
        tabWidget->addTab(filteredWidget, "Filtered Image");
    } else {
        filteredWidget->setImage(image);
    }
    tabWidget->setCurrentWidget(filteredWidget);
}

void MainWindow::setScaledImage(QImage *image)
{
    if (!scaledWidget) {
        scaledWidget = new ImageWidget(image);
        tabWidget->addTab(scaledWidget, "Scaled Image");
    } else {
        delete scaledWidget->getImage();
        scaledWidget->setImage(image);
    }
    tabWidget->setCurrentWidget(scaledWidget);
}

void MainWindow::setRotatedImage(QImage *image)
{
    if (!rotatedWidget) {
        rotatedWidget = new ImageWidget(image);
        tabWidget->addTab(rotatedWidget, "Rotated Image");
    } else {
        delete rotatedWidget->getImage();
        rotatedWidget->setImage(image);
    }
    tabWidget->setCurrentWidget(rotatedWidget);
}

void MainWindow::setAlgebraImage(QImage* image)
{
    if (!algebraWidget) {
        algebraWidget = new ImageWidget(image);
        tabWidget->addTab(algebraWidget, "Algebra");
    } else {
        delete algebraWidget->getImage();
        algebraWidget->setImage(image);
    }
    tabWidget->setCurrentWidget(algebraWidget);
}

void MainWindow::setContrastImage(QImage* image)
{
    if (!contrastWidget) {
        contrastWidget = new ImageWidget(image);
        tabWidget->addTab(contrastWidget, "Contrast");
    } else {
        contrastWidget->setImage(image);
    }
    tabWidget->setCurrentWidget(contrastWidget);
}

void MainWindow::setBrightnessImage(QImage* image)
{
    if (!brightWidget) {
        brightWidget = new ImageWidget(image);
        tabWidget->addTab(brightWidget, "Brightness");
    } else {
        brightWidget->setImage(image);
    }
    tabWidget->setCurrentWidget(brightWidget);
}

void MainWindow::on_actionOpen_triggered()
{
    imagePath = QFileDialog::getOpenFileName(
                this, tr("Open an Image"), QDir::currentPath(),
                tr("Image files(*.bmp *.jpeg *.jpg *.png *.gif);;All files (*.*)"));

    // load the image and update UI
    if (!imagePath.isNull()) {
        // set imageProcessor
        if (imageProcessor) {
            imageProcessor->setImage(imagePath);
        } else {
            imageProcessor = new ImageProcessor(imagePath);
        }

        if (isFirstImage) {
            if (tabWidget) {
                delete tabWidget;
            }
            tabWidget = new QTabWidget(this);
        } else {
            // remove previous tabs
            int amt = tabWidget->count();
            for (int i = 0; i < amt; ++i) {
                tabWidget->removeTab(0);
            }
        }

        // add a tab containing the image
        if (originWidget) {
            delete originWidget;
        }
        originWidget = new ImageWidget(
                    imageProcessor->getOriginImage(), tabWidget);
        tabWidget->addTab(originWidget, "Origin Image");

        if (isFirstImage) {
            // add tabWidget only if it is the first image opened
            ui->gridLayout->addWidget(tabWidget);
            // enable UI components
            ui->actionHistogram->setEnabled(true);
            ui->actionFilter->setEnabled(true);
            ui->actionScale->setEnabled(true);
            ui->actionRotate->setEnabled(true);
            ui->actionAlgebra->setEnabled(true);
            ui->actionEqualization->setEnabled(true);
            ui->actionGray_Scale->setEnabled(true);
#ifdef TEAM_WORK
            ui->actionBrightness->setEnabled(true);
            ui->actionContrast->setEnabled(true);
#endif
        } else {
            // reset UI components
            if (grayWidget) {
                delete grayWidget;
                grayWidget = 0;
            }
            if (binaryWidget) {
                delete binaryWidget;
                binaryWidget = 0;
            }
            if (filteredWidget) {
                delete filteredWidget;
                filteredWidget = 0;
            }
            if (scaledWidget) {
                delete scaledWidget->getImage();
                delete scaledWidget;
                scaledWidget = 0;
            }
            if (rotatedWidget) {
                delete rotatedWidget->getImage();
                delete rotatedWidget;
                rotatedWidget = 0;
            }
            if (algebraWidget) {
                delete algebraWidget->getImage();
                delete algebraWidget;
                algebraWidget = 0;
            }
            if (equalWidget) {
                delete equalWidget;
                equalWidget = 0;
            }
            if (contrastWidget) {
                delete contrastWidget;
                contrastWidget = 0;
            }
            if (brightWidget) {
                delete brightWidget;
                brightWidget = 0;
            }
            if (histogramDialog) {
                delete histogramDialog;
                histogramDialog = 0;
            }
            if (filterDialog) {
                delete filterDialog;
                filterDialog = 0;
            }
            if (scaledDialog) {
                delete scaledDialog;
                scaledDialog = 0;
            }
            if (rotatedDialog) {
                delete rotatedDialog;
                rotatedDialog = 0;
            }
            if (algebraDialog) {
                delete algebraDialog;
                algebraDialog = 0;
            }
            if (contrastDialog) {
                delete contrastDialog;
                contrastDialog = 0;
            }
        }

        // isFirstImage is set to be false after first opening
        isFirstImage = false;
    }
}

void MainWindow::on_actionHistogram_triggered()
{
    QImage* binaryImage = imageProcessor->getBinaryImage();
    if (binaryWidget) {
        delete binaryWidget;
    }
    binaryWidget = new ImageWidget(binaryImage, tabWidget);
    tabWidget->addTab(binaryWidget, "Binary Image");
    tabWidget->setCurrentWidget(binaryWidget);

    if (histogramDialog) {
        delete histogramDialog;
    }
    histogramDialog = new HistogramDialog(this, imageProcessor, this);
    histogramDialog->show();
}

void MainWindow::on_actionFilter_triggered()
{
    if (filterDialog) {
        delete filterDialog;
    }
    filterDialog = new FilterDialog(this, imageProcessor, this);
    filterDialog->show();
}

void MainWindow::on_actionBrightness_triggered()
{
#ifdef TEAM_WORK
    if (brightDialog) {
        delete brightDialog;
    }
    brightDialog = new BrightDialog(this, this);
    brightDialog->show();
#endif
}

void MainWindow::on_actionContrast_triggered()
{
#ifdef TEAM_WORK
    if (contrastDialog) {
        delete contrastDialog;
    }
    contrastDialog = new ContrastDialog(this, this);
    contrastDialog->show();
#endif
}

void MainWindow::on_actionScale_triggered()
{
    if (scaledDialog) {
        delete scaledDialog;
    }
    scaledDialog = new ScaleDialog(this, this);
    scaledDialog->show();
}

void MainWindow::on_actionRotate_triggered()
{
    if (rotatedDialog) {
        delete rotatedDialog;
    }
    rotatedDialog = new RotateDialog(this, this);
    rotatedDialog->show();
}

void MainWindow::on_actionAlgebra_triggered()
{
    if (algebraDialog) {
        delete algebraDialog;
    }
    algebraDialog = new AlgebraDialog(this);
    algebraDialog->show();
}

void MainWindow::on_actionEqualization_triggered()
{
    if (!equalWidget) {
        equalWidget = new ImageWidget(imageProcessor->getEqualImage());
        tabWidget->addTab(equalWidget, "Histogram Equalization");
    }
    tabWidget->setCurrentWidget(equalWidget);
}

void MainWindow::on_actionGray_Scale_triggered()
{
    if (!grayWidget) {
        grayWidget = new ImageWidget(imageProcessor->getGrayScaleImage());
        tabWidget->addTab(grayWidget, "Gray Scale");
    }
    tabWidget->setCurrentWidget(grayWidget);
}
