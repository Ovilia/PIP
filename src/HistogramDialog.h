#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QDialog>

#include "HistogramPlot.h"
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

private slots:

    void on_otsuButton_clicked();

    void on_entropyButton_clicked();

    void on_customedButton_clicked();

    void on_lowerSlider_sliderMoved(int position);

    void on_higherSlider_sliderMoved(int position);

private:
    ImageProcessor* imageProcessor;

    Ui::HistogramDialog *ui;
    HistogramPlot* histogramPlot;
};

#endif // HISTOGRAMDIALOG_H
