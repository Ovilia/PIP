#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QDialog>

#include "HistogramPlot.h"
#include "ImagePolicy.h"
#include "ImageProcessor.h"

namespace Ui {
    class HistogramDialog;
}

class HistogramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramDialog(ImageProcessor* imageProcessor,
                             QWidget *parent = 0);
    ~HistogramDialog();

    void changeThreshold(ImagePolicy::ThresholdPolicy policy,
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

private:
    ImageProcessor* imageProcessor;

    Ui::HistogramDialog *ui;
    HistogramPlot* histogramPlot;
};

#endif // HISTOGRAMDIALOG_H
