#include "RotateDialog.h"
#include "ui_RotateDialog.h"

#include <QString>

#include "ImageRotator.h"

RotateDialog::RotateDialog(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RotateDialog),
    mainWindow(mainWindow)
{
    ui->setupUi(this);
}

RotateDialog::~RotateDialog()
{
    delete ui;
}

void RotateDialog::on_angleSlider_sliderMoved(int position)
{
    ui->angleLabel->setText(QString::number(position));
}

void RotateDialog::on_angleSlider_sliderReleased()
{
    mainWindow->setRotatedImage(
                ImageRotator::getRotatedImage(
                    mainWindow->getImageProcessor()->getOriginImage(),
                    ui->angleSlider->value()));
}
