#include "HistogramDialog.h"
#include "HistogramPlot.h"
#include "ImagePolicy.h"
#include "ImageProcessor.h"
#include "ui_HistogramDialog.h"

HistogramDialog::HistogramDialog(
        MainWindow* mainWindow,
        ImageProcessor* imageProcessor,
        QWidget *parent) :
    QDialog(parent),
    mainWindow(mainWindow),
    imageProcessor(imageProcessor),
    ui(new Ui::HistogramDialog)
{
    ui->setupUi(this);

    histogramPlot = new HistogramPlot(imageProcessor->getHistogram());
    histogramPlot->setRgbDataset(imageProcessor->getRgbHistogram());
    ui->painterLayout->addWidget(histogramPlot);
    ui->rightWidget->repaint();

    changeThreshold(ImagePolicy::OTSU, false);
}

HistogramDialog::~HistogramDialog()
{
    delete ui;
    if (histogramPlot) {
        delete histogramPlot;
    }
    // remeber not to delete imageProcessor here,
    // because it is deleted by mainwindow
}

void HistogramDialog::changeThreshold(
        ImagePolicy::ThresholdPolicy policy,
        bool repaintBinary, int lower, int higher)
{
    // customed value set by parameter
    if (policy == ImagePolicy::COSTUMED) {
        imageProcessor->setThresholdPolicy(policy, lower, higher);
    } else {
        imageProcessor->setThresholdPolicy(policy);
        lower = imageProcessor->getLowerThreshold();
        higher = imageProcessor->getHigherThreshold();
    }
    ui->lowerSlider->setValue(lower);
    ui->higherSlider->setValue(higher);
    ui->lowerEdit->setText(QString::number(lower));
    ui->higherEdit->setText(QString::number(higher));
    histogramPlot->setThresholdValue(lower, higher);
    histogramPlot->repaint();

    // repaint binary image
    if (repaintBinary) {
        mainWindow->repaintBinary();
    }
}

void HistogramDialog::on_otsuButton_clicked()
{
    changeThreshold(ImagePolicy::OTSU);
    ui->lowerSlider->setEnabled(false);
    ui->higherSlider->setEnabled(false);
}

void HistogramDialog::on_entropyButton_clicked()
{
    changeThreshold(ImagePolicy::ENTROPY);
    ui->lowerSlider->setEnabled(false);
    ui->higherSlider->setEnabled(false);
}

void HistogramDialog::on_customedButton_clicked()
{
    changeThreshold(ImagePolicy::COSTUMED,
                    true,
                    ui->lowerSlider->value(),
                    ui->higherSlider->value());
    ui->lowerSlider->setEnabled(true);
    ui->higherSlider->setEnabled(true);
}

void HistogramDialog::on_lowerSlider_sliderMoved(int position)
{
    ui->lowerEdit->setText(QString::number(position));
    if (position > ui->higherSlider->value()) {
        ui->higherSlider->setValue(position);
        ui->higherEdit->setText(QString::number(position));
    }
    int lower = position;
    int higher = ui->higherSlider->value();
    imageProcessor->setThresholdPolicy(ImagePolicy::COSTUMED, lower, higher);
    histogramPlot->setThresholdValue(lower, higher);
    histogramPlot->repaint();

    // repaint binary image
    mainWindow->repaintBinary();
}

void HistogramDialog::on_higherSlider_sliderMoved(int position)
{
    ui->higherEdit->setText(QString::number(position));
    if (position < ui->lowerSlider->value()) {
        ui->lowerSlider->setValue(position);
        ui->lowerEdit->setText(QString::number(position));
    }
    int lower = ui->lowerSlider->value();
    int higher = position;
    imageProcessor->setThresholdPolicy(ImagePolicy::COSTUMED, lower, higher);
    histogramPlot->setThresholdValue(lower, higher);
    histogramPlot->repaint();

    // repaint binary image
    mainWindow->repaintBinary();
}

void HistogramDialog::on_redBox_clicked(bool checked)
{
    histogramPlot->setRedVisible(checked);
    histogramPlot->repaint();
}

void HistogramDialog::on_greenBox_clicked(bool checked)
{
    histogramPlot->setGreenVisible(checked);
    histogramPlot->repaint();
}

void HistogramDialog::on_blueBox_clicked(bool checked)
{
    histogramPlot->setBlueVisible(checked);
    histogramPlot->repaint();
}

void HistogramDialog::on_grayBox_clicked(bool checked)
{
    histogramPlot->setGrayVisible(checked);
    histogramPlot->repaint();
}

void HistogramDialog::changeGrayScale(ImagePolicy::GrayScalePolicy policy)
{
    // change gray scale policy
    imageProcessor->setGrayScalePolicy(policy);
    histogramPlot->setGrayDataset(imageProcessor->getHistogram());

    // recompute threshold value
    ui->otsuButton->setChecked(true);
    changeThreshold(ImagePolicy::OTSU);
}

void HistogramDialog::on_luminanceButton_clicked(bool checked)
{
    if (checked) {
        changeGrayScale(ImagePolicy::MATCH_LUMINANCE);
    }
}

void HistogramDialog::on_rgbButton_clicked(bool checked)
{
    if (checked) {
        changeGrayScale(ImagePolicy::RGB_AVERAGE);
    }
}

void HistogramDialog::on_greenButton_clicked(bool checked)
{
    if (checked) {
        changeGrayScale(ImagePolicy::GREEN_ONLY);
    }
}

void HistogramDialog::on_lowerSlider_sliderPressed()
{
    if (ui->transparenBox->isChecked()) {
        setWindowOpacity(0.5);
    }
}

void HistogramDialog::on_lowerSlider_sliderReleased()
{
    setWindowOpacity(1.0);
}

void HistogramDialog::on_higherSlider_sliderPressed()
{
    if (ui->transparenBox->isChecked()) {
        setWindowOpacity(0.5);
    }
}

void HistogramDialog::on_higherSlider_sliderReleased()
{
    setWindowOpacity(1.0);
}
