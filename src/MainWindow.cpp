#include <QFileDialog>
#include <QPixmap>

#include "HistogramDialog.h"
#include "ImagePolicy.h"
#include "mainwindow.h"
#include "ui_MainWindow.h"

//#define TEAM_WORK

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabWidget(0),
    originWidget(0),
    binaryWidget(0),
    filteredWidget(0),
    histogramDialog(0),
    filterDialog(0),
    imageProcessor(0),
    isFirstImage(true)
{
    ui->setupUi(this);

#ifndef TEAM_WORK
    ui->actionBrightness->setVisible(false);
    ui->actionContrast->setVisible(false);
    ui->actionUndo->setVisible(false);
    ui->actionRedo->setVisible(false);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
    if (originWidget) {
        delete originWidget;
    }
    if (binaryWidget) {
        delete binaryWidget;
    }
    if (filteredWidget) {
        delete filteredWidget;
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
    if (imageProcessor) {
        delete imageProcessor;
    }
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
#ifdef TEAM_WORK
            ui->actionBrightness->setEnabled(true);
            ui->actionContrast->setEnabled(true);
#endif
        } else {
            // reset UI components
            if (binaryWidget) {
                delete binaryWidget;
                binaryWidget = 0;
            }
            if (filteredWidget) {
                delete filteredWidget;
                filteredWidget = 0;
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
    // TODO: do contrast here
    imageProcessor->doContrast(50);
    ui->actionUndo->setEnabled(true);
#endif
}

void MainWindow::on_actionContrast_triggered()
{
#ifdef TEAM_WORK
    // TODO: do contrast here
    imageProcessor->doBrightness(0);
    ui->actionUndo->setEnabled(true);
#endif
}

void MainWindow::on_actionUndo_triggered()
{
    ui->actionUndo->setEnabled(imageProcessor->undo());
    ui->actionRedo->setEnabled(imageProcessor->isRedoable());
}

void MainWindow::on_actionRedo_triggered()
{
    ui->actionRedo->setEnabled(imageProcessor->redo());
    ui->actionUndo->setEnabled(imageProcessor->isUndoable());
}
