#include <QString>

#include "ContrastDialog.h"
#include "ui_ContrastDialog.h"

ContrastDialog::ContrastDialog(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContrastDialog),
    mainWindow(mainWindow)
{
    ui->setupUi(this);
}

ContrastDialog::~ContrastDialog()
{
    delete ui;
}

void ContrastDialog::on_horizontalSlider_valueChanged(int value)
{
    mainWindow->setContrastImage(
                mainWindow->getImageProcessor()->getContrastImage(value));
    ui->valueLabel->setText(QString::number(value));
}
