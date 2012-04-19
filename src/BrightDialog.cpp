#include "BrightDialog.h"
#include "ui_BrightDialog.h"

#include <QString>

BrightDialog::BrightDialog(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrightDialog),
    mainWindow(mainWindow)
{
    ui->setupUi(this);
}

BrightDialog::~BrightDialog()
{
    delete ui;
}

void BrightDialog::on_brightSlider_valueChanged(int value)
{
    ui->brightLabel->setText(QString::number(value));
    mainWindow->setBrightnessImage(
                mainWindow->getImageProcessor()->getBrightnessImage(value));
}
