#include <QLineEdit>
#include <QList>

#include "FilterDialog.h"
#include "MainWindow.h"
#include "ui_FilterDialog.h"

FilterDialog::FilterDialog(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    ui->filterParaGroup->setVisible(false);
    ui->gaussianParaGroup->setVisible(false);
    ui->customedParaGroup->setVisible(false);

    for (int i = 0; i < MAX_KERNEL_RADIO; ++i) {
        for (int j = 0; j < MAX_KERNEL_RADIO; ++j) {
            int index = i * MAX_KERNEL_RADIO + j;
            kernelEdit[index] = new QLineEdit("0");
            ui->gridLayout->addWidget(kernelEdit[index], i, j, 1, 1);
        }
    }
    resetCustEdit(MIN_KERNEL_RADIO);
}

FilterDialog::~FilterDialog()
{
    int amt = MAX_KERNEL_RADIO * MAX_KERNEL_RADIO;
    for (int i = 0; i < amt; ++i) {
        delete kernelEdit[i];
    }

    delete ui;
}

void FilterDialog::disableAllButtons()
{
    ui->robertsButton->setChecked(false);
    ui->prewittButton->setChecked(false);
    ui->sobelButton->setChecked(false);
    ui->gaussianButton->setChecked(false);
    ui->meanButton->setChecked(false);
    ui->medianFilter->setChecked(false);
    ui->customedButton->setChecked(false);
    ui->filterParaGroup->setVisible(false);
    ui->gaussianParaGroup->setVisible(false);
    ui->customedParaGroup->setVisible(false);
}

void FilterDialog::on_robertsButton_clicked(bool checked)
{
    if (checked) {
        disableAllButtons();
        ui->robertsButton->setChecked(true);
    }
}

void FilterDialog::on_prewittButton_clicked(bool checked)
{
    if (checked) {
        disableAllButtons();
        ui->prewittButton->setChecked(true);
    }
}

void FilterDialog::on_sobelButton_clicked(bool checked)
{
    if (checked) {
        disableAllButtons();
        ui->sobelButton->setChecked(true);
    }
}

void FilterDialog::on_gaussianButton_clicked(bool checked)
{
    if (checked) {
        disableAllButtons();
        ui->gaussianButton->setChecked(true);
        ui->gaussianParaGroup->setVisible(true);
        ui->filterParaGroup->setVisible(true);
    }
}

void FilterDialog::on_meanButton_clicked(bool checked)
{
    if (checked) {
        disableAllButtons();
        ui->meanButton->setChecked(true);
        ui->filterParaGroup->setVisible(true);
    }
}

void FilterDialog::on_medianFilter_clicked(bool checked)
{
    if (checked) {
        disableAllButtons();
        ui->medianFilter->setChecked(true);
        ui->filterParaGroup->setVisible(true);
    }
}

void FilterDialog::on_customedButton_clicked(bool checked)
{
    if (checked) {
        disableAllButtons();
        ui->customedButton->setChecked(true);
        ui->filterParaGroup->setVisible(true);
        ui->customedParaGroup->setVisible(true);
        ui->filterRadioSpin->setValue(MIN_KERNEL_RADIO);
    }
}

void FilterDialog::on_filterRadioSpin_valueChanged(int value)
{
    if (value % 2 == 0) {
        // even radio is not allowed
        if (value - 1 >= MIN_KERNEL_RADIO) {
            value = value - 1;
            ui->filterRadioSpin->setValue(value);
        } else {
            value = MIN_KERNEL_RADIO;
            ui->filterRadioSpin->setValue(value);
        }
    }
    // update customed kernel if customedButton is checked
    if (ui->customedButton->isChecked()) {
        if (value < MIN_KERNEL_RADIO) {
            ui->filterRadioSpin->setValue(MIN_KERNEL_RADIO);
        } else if (value > MAX_KERNEL_RADIO) {
            ui->filterRadioSpin->setValue(MAX_KERNEL_RADIO);
        }
        resetCustEdit(value);
    }
}

void FilterDialog::resetCustEdit(int count)
{
    for (int i = 0; i < MAX_KERNEL_RADIO; ++i) {
        for (int j = 0; j < MAX_KERNEL_RADIO; ++j) {
            if (i < count && j < count) {
                kernelEdit[i * MAX_KERNEL_RADIO + j]->setVisible(true);
            } else {
                kernelEdit[i * MAX_KERNEL_RADIO + j]->setVisible(false);
            }
        }
    }
}

void FilterDialog::on_applyButton_clicked()
{
}
