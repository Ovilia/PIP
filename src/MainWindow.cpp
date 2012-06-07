#define TEAM_WORK

#include <QFileDialog>
#include <QMessageBox>
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
    morphoWidget(0),
    distanceWidget(0),
    skeletonWidget(0),
    reconstWidget(0),
    edgeWidget(0),
    retinaWidget(0),
    binReconsWidget(0),
    discWidget(0),

    histogramDialog(0),
    filterDialog(0),
    scaledDialog(0),
    rotatedDialog(0),
    algebraDialog(0),
    contrastDialog(0),
    brightDialog(0),
    morphoDialog(0),

    imageProcessor(0),
    retinaProcessor(0),

    binaryMorphology(0),
    morphoDistance(0),
    useSquareSe(true),

    binMorpho(0),

    grayMorphology(0),

    reconstructedImage(0),

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
    if (morphoWidget) {
        delete morphoWidget;
    }
    if (distanceWidget) {
        delete distanceWidget;
    }
    if (skeletonWidget) {
        delete skeletonWidget;
    }
    if (reconstWidget) {
        delete reconstWidget;
    }
    if (edgeWidget) {
        delete edgeWidget;
    }
    if (retinaWidget) {
        delete retinaWidget;
    }
    if (binReconsWidget) {
        delete binReconsWidget;
    }
    if (discWidget) {
        delete discWidget;
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
    if (morphoDialog) {
        delete morphoDialog;
    }

    if (binaryMorphology) {
        delete binaryMorphology;
    }
    if (morphoDistance) {
        delete morphoDistance;
    }
    if (binMorpho) {
        delete binMorpho;
    }

    if (grayMorphology) {
        delete grayMorphology;
    }

    if (imageProcessor) {
        delete imageProcessor;
    }
    if (retinaProcessor) {
        delete retinaProcessor;
    }

    if (reconstructedImage) {
        delete reconstructedImage;
    }
}

ImageProcessor* MainWindow::getImageProcessor()
{
    return imageProcessor;
}

BinaryMorphology* MainWindow::getBinaryMorpo()
{
    if (!binaryMorphology) {
        binaryMorphology = new BinaryMorphology(imageProcessor);
    }
    return binaryMorphology;
}

GrayMorphology* MainWindow::getGrayMorpo()
{
    if (!grayMorphology) {
        grayMorphology = new GrayMorphology(imageProcessor);
    }
    return grayMorphology;
}

void MainWindow::repaintBinary()
{
    binaryWidget->setImage(imageProcessor->getBinaryImage());
    binaryWidget->repaint();
}

void MainWindow::setFilteredImage(QImage *image)
{
    if (!filteredWidget) {
        filteredWidget = new ImageWidget(this, image);
        tabWidget->addTab(filteredWidget, "Filtered Image");
    } else {
        filteredWidget->setImage(image);
    }
    tabWidget->setCurrentWidget(filteredWidget);
}

void MainWindow::setScaledImage(QImage *image)
{
    if (!scaledWidget) {
        scaledWidget = new ImageWidget(this, image);
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
        rotatedWidget = new ImageWidget(this, image);
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
        algebraWidget = new ImageWidget(this, image);
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
        contrastWidget = new ImageWidget(this, image);
        tabWidget->addTab(contrastWidget, "Contrast");
    } else {
        contrastWidget->setImage(image);
    }
    tabWidget->setCurrentWidget(contrastWidget);
}

void MainWindow::setBrightnessImage(QImage* image)
{
    if (!brightWidget) {
        brightWidget = new ImageWidget(this, image);
        tabWidget->addTab(brightWidget, "Brightness");
    } else {
        brightWidget->setImage(image);
    }
    tabWidget->setCurrentWidget(brightWidget);
}

void MainWindow::setMorphologyImage(QImage *image)
{
    if (!morphoWidget) {
        morphoWidget = new ImageWidget(this, image);
        tabWidget->addTab(morphoWidget, "Morphology");
    } else {
        morphoWidget->setImage(image);
    }
    tabWidget->setCurrentWidget(morphoWidget);
}

void MainWindow::on_actionOpen_triggered()
{
    imagePath = QFileDialog::getOpenFileName(
                this, tr("Open an Image"), QDir::currentPath(),
                tr("Image files(*.bmp *.jpeg *.jpg *.png *.gif *.tif);;"\
                   "All files (*.*)"));

    // load the image and update UI
    if (!imagePath.isNull()) {
        resetImage(imagePath);
    }
}

void MainWindow::resetImage(const QString &fileName)
{
    // set imageProcessor
    if (imageProcessor) {
        delete imageProcessor;
    }
    imageProcessor = new ImageProcessor(fileName);
    resetImage();
}

void MainWindow::resetImage(QImage* image)
{
    // set imageProcessor
    // image may possible be part of old imageProcessor,
    // copy before delete old imageProcessor
    ImageProcessor* newProcessor = new ImageProcessor(image);
    if (imageProcessor) {
        delete imageProcessor;
    }
    imageProcessor = newProcessor;
    resetImage();
}

void MainWindow::resetImage()
{
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
                this, imageProcessor->getOriginImage(), tabWidget);
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
        ui->actionMorpOper->setEnabled(true);
        ui->actionDistance->setEnabled(true);
        ui->actionSkeleton->setEnabled(true);
        ui->actionReconstruct->setEnabled(true);
        ui->actionExternal->setEnabled(true);
        ui->actionInternal->setEnabled(true);
        ui->actionStandard->setEnabled(true);
        ui->actionProcess->setEnabled(true);
        ui->actionManual_Process->setEnabled(true);
        ui->actionBinRecons->setEnabled(true);
        ui->actionOptic_Disc->setEnabled(true);
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
        if (morphoWidget) {
            delete morphoWidget;
            morphoWidget = 0;
        }
        if (distanceWidget) {
            delete distanceWidget;
            distanceWidget = 0;
        }
        if (skeletonWidget) {
            delete skeletonWidget;
            skeletonWidget = 0;
        }
        if (reconstWidget) {
            delete reconstWidget;
            reconstWidget = 0;
        }
        if (edgeWidget) {
            delete edgeWidget;
            edgeWidget = 0;
        }
        if (retinaWidget) {
            delete retinaWidget;
            retinaWidget = 0;
        }
        if (binReconsWidget) {
            delete binReconsWidget;
            binReconsWidget = 0;
        }
        if (discWidget) {
            delete discWidget;
            discWidget = 0;
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
        if (morphoDialog) {
            delete morphoDialog;
            morphoDialog = 0;
        }

        if (binaryMorphology) {
            delete binaryMorphology;
            binaryMorphology = 0;
        }
        if (morphoDistance) {
            delete morphoDistance;
            morphoDistance = 0;
        }
        useSquareSe = true;
        if (binMorpho) {
            delete binMorpho;
            binMorpho = 0;
        }

        if (grayMorphology) {
            delete grayMorphology;
            grayMorphology = 0;
        }

        if (reconstructedImage) {
            delete reconstructedImage;
            reconstructedImage = 0;
        }

        if (retinaProcessor) {
            delete retinaProcessor;
            retinaProcessor = 0;
        }
    }

    // isFirstImage is set to be false after first opening
    isFirstImage = false;
}

void MainWindow::on_actionHistogram_triggered()
{
    QImage* binaryImage = imageProcessor->getBinaryImage();
    if (binaryWidget) {
        delete binaryWidget;
    }
    binaryWidget = new ImageWidget(this, binaryImage, tabWidget);
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
        equalWidget = new ImageWidget(this, imageProcessor->getEqualImage());
        tabWidget->addTab(equalWidget, "Histogram Equalization");
    }
    tabWidget->setCurrentWidget(equalWidget);
}

void MainWindow::on_actionGray_Scale_triggered()
{
    if (!grayWidget) {
        grayWidget = new ImageWidget(this, imageProcessor->getGrayScaleImage());
        tabWidget->addTab(grayWidget, "Gray Scale");
    }
    tabWidget->setCurrentWidget(grayWidget);
}

void MainWindow::on_actionMorpOper_triggered()
{
    if (!morphoDialog) {
        morphoDialog = new MorphologyDialog(this, this);
    }
    morphoDialog->show();
}

void MainWindow::on_actionDistance_triggered()
{
    if (QMessageBox::question(this, "Distance SE",
                              "Do you want to use squre SE?\n"\
                              "Yes for Squre SE. No for Cross SE.",
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes) == QMessageBox::Yes) {
        useSquareSe = true;
    } else {
        useSquareSe = false;
    }

    if (!morphoDistance) {
        morphoDistance = new MorphoDistance(imageProcessor);
    }
    if (!distanceWidget) {
        distanceWidget = new ImageWidget(this,
                    morphoDistance->getDistanceImage(useSquareSe));
        tabWidget->addTab(distanceWidget, "Distance");
    } else if (useSquareSe != morphoDistance->isUseSquareSe()) {
        tabWidget->removeTab(tabWidget->indexOf(distanceWidget));
        distanceWidget = new ImageWidget(this,
                    morphoDistance->getDistanceImage(useSquareSe));
        tabWidget->addTab(distanceWidget, "Distance");
    }
    tabWidget->setCurrentWidget(distanceWidget);
}

void MainWindow::on_actionSkeleton_triggered()
{
    if (!morphoDistance) {
        morphoDistance = new MorphoDistance(imageProcessor);
    }
    if (!skeletonWidget) {
        skeletonWidget = new ImageWidget(this, morphoDistance->getSkeletonImage());
        tabWidget->addTab(skeletonWidget, "Skeleton");
    }
    tabWidget->setCurrentWidget(skeletonWidget);
}

void MainWindow::on_actionReconstruct_triggered()
{
    if (!morphoDistance) {
        morphoDistance = new MorphoDistance(imageProcessor);
    }
    if (!reconstWidget) {
        if (!reconstructedImage) {
            QImage* origin = imageProcessor->getOriginImage();
            reconstructedImage = new QImage(origin->size(), origin->format());
        }
        morphoDistance->getSkeletonImage(reconstructedImage);
        reconstWidget = new ImageWidget(this, reconstructedImage);
        tabWidget->addTab(reconstWidget, "Recontruction");
    }
    tabWidget->setCurrentWidget(reconstWidget);
}

void MainWindow::on_actionStandard_triggered()
{
    if (!binMorpho) {
        delete binMorpho;
    }
    binMorpho = new BinaryMorphology(imageProcessor);
    if (edgeWidget) {
        tabWidget->removeTab(tabWidget->indexOf(edgeWidget));
        delete edgeWidget;
    }
    StructElement se(3, StructElement::ST_CROSS);
    edgeWidget = new ImageWidget(this, binMorpho->getEdgeImage(
                                     se, BinaryMorphology::ET_STANDARD));
    tabWidget->addTab(edgeWidget, "Standard Edge");
    tabWidget->setCurrentWidget(edgeWidget);
}

void MainWindow::on_actionInternal_triggered()
{
    if (!binMorpho) {
        delete binMorpho;
    }
    binMorpho = new BinaryMorphology(imageProcessor);
    if (edgeWidget) {
        tabWidget->removeTab(tabWidget->indexOf(edgeWidget));
        delete edgeWidget;
    }
    StructElement se(3, StructElement::ST_CROSS);
    edgeWidget = new ImageWidget(this, binMorpho->getEdgeImage(
                                     se, BinaryMorphology::ET_INTERNAL));
    tabWidget->addTab(edgeWidget, "Internal Edge");
    tabWidget->setCurrentWidget(edgeWidget);
}

void MainWindow::on_actionExternal_triggered()
{
    if (!binMorpho) {
        delete binMorpho;
    }
    binMorpho = new BinaryMorphology(imageProcessor);

    if (edgeWidget) {
        tabWidget->removeTab(tabWidget->indexOf(edgeWidget));
        delete edgeWidget;
    }
    StructElement se(3, StructElement::ST_CROSS);
    edgeWidget = new ImageWidget(this, binMorpho->getEdgeImage(
                                     se, BinaryMorphology::ET_EXTERNAL));
    tabWidget->addTab(edgeWidget, "External Edge");
    tabWidget->setCurrentWidget(edgeWidget);
}

void MainWindow::on_actionProcess_triggered()
{
    if (!retinaProcessor) {
        QString maskFile = QFileDialog::getOpenFileName(
                    this, tr("Open a Mask"), QDir::currentPath(),
                    tr("Image files(*.bmp *.jpeg *.jpg *.png *.gif *.tif);;"\
                       "All files (*.*)"));
        if (maskFile.isNull()) {
            return;
        }
        ImageProcessor maskProcessor(maskFile);
        retinaProcessor = new RetinaProcessor(imageProcessor->getOriginImage(),
                                              maskProcessor.getBinaryImage());
    }
    if (!retinaWidget) {
        retinaWidget = new ImageWidget(this, retinaProcessor->getRetinaImage(),
                                       this);
        tabWidget->addTab(retinaWidget, "Retina");
    }
    tabWidget->setCurrentWidget(retinaWidget);
}

void MainWindow::on_actionManual_Process_triggered()
{
    if (retinaProcessor) {
        delete retinaProcessor;
    }
    QString maskFile = QFileDialog::getOpenFileName(
                this, tr("Open a Mask"), QDir::currentPath(),
                tr("Image files(*.bmp *.jpeg *.jpg *.png *.gif *.tif);;"\
                   "All files (*.*)"));
    if (maskFile.isNull()) {
        return;
    }
    ImageProcessor maskProcessor(maskFile);
    retinaProcessor = new RetinaProcessor(imageProcessor->getOriginImage(),
                                          maskProcessor.getBinaryImage());
    if (retinaWidget) {
        delete retinaWidget;
    }
    retinaWidget = new ImageWidget(this, retinaProcessor->getEqualImage(),
                                   this);
    tabWidget->addTab(retinaWidget, "Retina");
    tabWidget->setCurrentWidget(retinaWidget);
}

void MainWindow::on_actionBinRecons_triggered()
{
    if (binMorpho) {
        delete binMorpho;
    }
    binMorpho = new BinaryMorphology(imageProcessor);

    if (!binReconsWidget) {
        StructElement se(3, StructElement::ST_SQUARE);
        binReconsWidget = new ImageWidget(this,
                                          binMorpho->getReconstructImage(se),
                                          this);
        tabWidget->addTab(binReconsWidget, "Binary Reconstruction");
    }
    tabWidget->setCurrentWidget(binReconsWidget);
}

void MainWindow::on_actionOptic_Disc_triggered()
{
    if (!discWidget) {
        if (retinaProcessor) {
            delete retinaProcessor;
        }
        retinaProcessor = new RetinaProcessor(
                    imageProcessor->getOriginImage(), 0);
        discWidget = new ImageWidget(this, retinaProcessor->getCenterImage(),
                                     tabWidget);
        tabWidget->addTab(discWidget, "Optic Disc");
    }
    tabWidget->setCurrentWidget(discWidget);
}
