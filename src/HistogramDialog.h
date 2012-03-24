#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QDialog>

#include "HistogramPlot.h"
#include "ImagePolicy.h"
#include "ImageProcessor.h"
#include "MainWindow.h"

namespace Ui {
    class HistogramDialog;
}

class MainWindow;

class HistogramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramDialog(MainWindow* mainWindow,
                             ImageProcessor* imageProcessor,
                             QWidget *parent = 0);
    ~HistogramDialog();

    void changeThreshold(ImagePolicy::ThresholdPolicy policy,
                         bool repaintBinary = true,
                         int lower = 0, int higher = 0);
    void changeGrayScale(ImagePolicy::GrayScalePolicy policy);

private slots:

    void on_otsuButton_clicked();

    void on_entropyButton_clicked();

    void on_customedButton_clicked();

    void on_lowerSlider_sliderMoved(int position);

    void on_higherSlider_sliderMoved(int position);

    void on_redBox_clicked(bool checked);

    void on_greenBox_clicked(bool checked);

    void on_blueBox_clicked(bool checked);

    void on_grayBox_clicked(bool checked);

    void on_luminanceButton_clicked(bool checked);

    void on_rgbButton_clicked(bool checked);

    void on_greenButton_clicked(bool checked);

    void on_lowerSlider_sliderPressed();

    void on_lowerSlider_sliderReleased();

    void on_higherSlider_sliderPressed();

    void on_higherSlider_sliderReleased();

private:
    MainWindow* mainWindow;
    ImageProcessor* imageProcessor;

    Ui::HistogramDialog *ui;
    HistogramPlot* histogramPlot;
};

#endif // HISTOGRAMDIALOG_H
