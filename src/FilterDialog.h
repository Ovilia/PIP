#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QSpinBox>

#include "EmbossFilter.h"
#include "GaussianFilter.h"
#include "ImagePolicy.h"
#include "ImageProcessor.h"
#include "MainWindow.h"
#include "MeanFilter.h"
#include "MedianFilter.h"
#include "MotionFilter.h"
#include "PointFilter.h"
#include "PrewittOperator.h"
#include "RobertsOperator.h"
#include "SharpenFilter.h"
#include "SobelOperator.h"

#define TEAM_WORK

namespace Ui {
    class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(class MainWindow* mainWindow,
                          ImageProcessor* imageProcessor,
                          QWidget *parent = 0);
    ~FilterDialog();

private slots:
    void on_robertsButton_clicked(bool checked);

    void on_prewittButton_clicked(bool checked);

    void on_sobelButton_clicked(bool checked);

    void on_gaussianButton_clicked(bool checked);

    void on_meanButton_clicked(bool checked);

    void on_medianFilter_clicked(bool checked);

    void on_customedButton_clicked(bool checked);

#ifdef TEAM_WORK
    void on_sharpenButton_clicked();

    void on_embossButton_clicked();

    void on_motionButton_clicked();
#endif

    void on_filterRadioSpin_valueChanged(int value);

    void on_applyButton_clicked();

    void on_nearestButton_clicked();

    void on_mirrorButton_clicked();

    void on_periodicButton_clicked();

    void on_blackButton_clicked();

    void on_horizontalCheck_clicked(bool checked);

    void on_verticalCheck_clicked(bool checked);

    void on_rgbCheck_clicked(bool checked);

    void on_comicButton_clicked();

    void on_sepiaButton_clicked();

private:
    Ui::FilterDialog *ui;
    MainWindow* mainWindow;
    ImageProcessor* imageProcessor;

    static const int MIN_KERNEL_RADIO = 3;
    static const int MAX_KERNEL_RADIO = 9;
    QSpinBox* kernelSpin[MAX_KERNEL_RADIO * MAX_KERNEL_RADIO];

    ImagePolicy::BorderPolicy borderPolicy;

    bool isColored;
    // colored an not colored buffer
    PrewittOperator* prewitt[2];
    RobertsOperator* roberts[2];
    SobelOperator* sobel[2];
    GaussianFilter* gaussian[2];
    MeanFilter* mean[2];
    MedianFilter* median[2];
    LinearFilter* customed;
    int* customedPtr;

#ifdef TEAM_WORK
    SharpenFilter* sharpen[2];
    EmbossFilter* emboss[2];
    MotionFilter* motion[2];
    PointFilter* point;
#endif

    void disableAllButtons();
    void resetCustEdit(int count);
};

#endif // FILTERDIALOG_H
