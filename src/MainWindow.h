#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

#include "AlgebraDialog.h"
#include "BinaryMorphology.h"
#include "BrightDialog.h"
#include "ContrastDialog.h"
#include "FilterDialog.h"
#include "HistogramDialog.h"
#include "ImageProcessor.h"
#include "ImageWidget.h"
#include "MorphoDistance.h"
#include "MorphologyDialog.h"
#include "RotateDialog.h"
#include "ScaleDialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    ImageProcessor* getImageProcessor();
    BinaryMorphology* getBinaryMorpo();

    void repaintBinary();
    void setFilteredImage(QImage* image);
    void setScaledImage(QImage* image);
    void setRotatedImage(QImage *image);
    void setAlgebraImage(QImage* image);
    void setContrastImage(QImage* image);
    void setBrightnessImage(QImage* image);
    void setMorphologyImage(QImage* image);

private slots:
    void on_actionOpen_triggered();

    void on_actionHistogram_triggered();

    void on_actionFilter_triggered();

    void on_actionBrightness_triggered();

    void on_actionContrast_triggered();

    void on_actionScale_triggered();

    void on_actionAlgebra_triggered();

    void on_actionEqualization_triggered();

    void on_actionGray_Scale_triggered();

    void on_actionRotate_triggered();

    void on_actionMorpOper_triggered();

    void on_actionDistance_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget* tabWidget;
    ImageWidget* originWidget;
    ImageWidget* grayWidget;
    ImageWidget* binaryWidget;
    ImageWidget* filteredWidget;
    ImageWidget* scaledWidget;
    ImageWidget* rotatedWidget;
    ImageWidget* algebraWidget;
    ImageWidget* equalWidget;
    ImageWidget* contrastWidget;
    ImageWidget* brightWidget;
    ImageWidget* morphoWidget;
    ImageWidget* distanceWidget;

    class HistogramDialog* histogramDialog;
    class FilterDialog* filterDialog;
    class ScaleDialog* scaledDialog;
    class RotateDialog* rotatedDialog;
    class AlgebraDialog* algebraDialog;
    class ContrastDialog* contrastDialog;
    class BrightDialog* brightDialog;
    class MorphologyDialog* morphoDialog;

    // file location of image
    QString imagePath;
    ImageProcessor* imageProcessor;

    BinaryMorphology* binaryMorphology;
    MorphoDistance* morphoDistance;
    bool useSquareSe;

    // if is the first time opening an image
    bool isFirstImage;
};

#endif // MAINWINDOW_H
