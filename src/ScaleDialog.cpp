#include <QString>

#include "ScaleDialog.h"
#include "ui_ScaleDialog.h"

#include "ImagePolicy.h"
#include "ImageScaler.h"

ScaleDialog::ScaleDialog(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScaleDialog),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    originImage = mainWindow->getImageProcessor()->getOriginImage();
    int width = originImage->width();
    int height = originImage->height();
    ui->oWidthLabel->setText(QString::number(width));
    ui->oHeightLabel->setText(QString::number(height));
    if (ui->widthSpin->maximum() < width) {
        ui->widthSpin->setMaximum(width * 2);
    }
    if (ui->heightSpin->maximum() < height) {
        ui->heightSpin->setMaximum(height * 2);
    }
    ui->widthSpin->setValue(width);
    ui->heightSpin->setValue(height);
}

ScaleDialog::~ScaleDialog()
{
    delete ui;
}

void ScaleDialog::on_resetButton_clicked()
{
    int width = originImage->width();
    int height = originImage->height();
    ui->widthSpin->setValue(width);
    ui->heightSpin->setValue(height);
}

void ScaleDialog::on_applyButton_clicked()
{
    mainWindow->setScaledImage(ImageScaler::getScaledImage(
                                   originImage,
                                   ui->widthSpin->value(),
                                   ui->heightSpin->value(),
                                   ImagePolicy::SP_NEAREST));
    close();
}
