#include "HistogramDialog.h"
#include "ImageProcessor.h"
#include "ui_HistogramDialog.h"
#include <stdio.h>

HistogramDialog::HistogramDialog(ImageProcessor* imageProcessor,
                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistogramDialog),
    imageProcessor(imageProcessor)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

HistogramDialog::~HistogramDialog()
{
    delete ui;
}

void HistogramDialog::on_otsuButton_clicked()
{
    ui->lowerSlider->setValue(0);
    // TODO: set high value here
    ui->lowerSlider->setEnabled(false);
    ui->higherSlider->setEnabled(false);
}

void HistogramDialog::on_entropyButton_clicked()
{
    ui->lowerSlider->setValue(0);
    // TODO: set high value here
    ui->lowerSlider->setEnabled(false);
    ui->higherSlider->setEnabled(false);
}

void HistogramDialog::on_customedButton_clicked()
{
    ui->lowerSlider->setEnabled(true);
    ui->higherSlider->setEnabled(true);
}

void HistogramDialog::on_lowerSlider_sliderMoved(int position)
{
    ui->lowerEdit->setText(QString::number(position));
    if (position > ui->higherSlider->value()) {
        printf("lower - position[%d] > higher[%d]\n",
               position, ui->higherSlider->value());
        ui->higherSlider->setValue(position);
        ui->higherEdit->setText(QString::number(position));
    }
}

void HistogramDialog::on_higherSlider_sliderMoved(int position)
{
    ui->higherEdit->setText(QString::number(position));
    if (position < ui->lowerSlider->value()) {
        printf("higher - position[%d] < low[%d]\n",
               position, ui->lowerSlider->value());
        ui->lowerSlider->setValue(position);
        ui->lowerEdit->setText(QString::number(position));
    }
}
