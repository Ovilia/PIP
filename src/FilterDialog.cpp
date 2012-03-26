#include <QLineEdit>

#include "FilterDialog.h"
#include "ImagePolicy.h"
#include "MainWindow.h"
#include "ui_FilterDialog.h"

FilterDialog::FilterDialog(MainWindow* mainWindow,
                           ImageProcessor* imageProcessor,
                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog),
    mainWindow(mainWindow),
    imageProcessor(imageProcessor),
    borderPolicy(ImagePolicy::NEAREST),
    isColored(false)
{
    ui->setupUi(this);

    for (int i = 0; i < 2; ++i) {
        prewitt[i] = 0;
        roberts[i] = 0;
        sobel[i] = 0;
    }

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

    for (int i = 0; i < 2; ++i) {
        if (prewitt[i]) {
            delete prewitt[i];
        }
        if (roberts[i]) {
            delete roberts[i];
        }
        if (sobel[i]) {
            delete sobel[i];
        }
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
    ui->operatorParaGroup->setVisible(false);
    ui->filterParaGroup->setVisible(false);
    ui->gaussianParaGroup->setVisible(false);
    ui->customedParaGroup->setVisible(false);
}

void FilterDialog::on_robertsButton_clicked(bool checked)
{
    if (checked) {
        disableAllButtons();
        ui->robertsButton->setChecked(true);
        ui->operatorParaGroup->setVisible(true);
    }
}

void FilterDialog::on_prewittButton_clicked(bool checked)
{
    if (checked) {
        disableAllButtons();
        ui->prewittButton->setChecked(true);
        ui->operatorParaGroup->setVisible(true);
    }
}

void FilterDialog::on_sobelButton_clicked(bool checked)
{
    if (checked) {
        disableAllButtons();
        ui->sobelButton->setChecked(true);
        ui->operatorParaGroup->setVisible(true);
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
    if (ui->robertsButton->isChecked()) {
        if (!roberts[isColored]) {
            if (isColored) {
                roberts[isColored] = new RobertsOperator(
                            imageProcessor->getOriginImage(),
                            isColored, borderPolicy);
            } else {
                roberts[isColored] = new RobertsOperator(
                            imageProcessor->getGrayScaleImage(),
                            isColored, borderPolicy);
            }
        }
        roberts[isColored]->setXEnabled(ui->horizontalCheck->isChecked());
        roberts[isColored]->setYEnabled(ui->verticalCheck->isChecked());
        mainWindow->setFilteredImage(roberts[isColored]->getFilteredImage());

    } else if (ui->prewittButton->isChecked()) {
        if (isColored) {
            prewitt[isColored] = new PrewittOperator(
                        imageProcessor->getOriginImage(),
                        isColored, borderPolicy);
        } else {
            prewitt[isColored] = new PrewittOperator(
                        imageProcessor->getGrayScaleImage(),
                        isColored, borderPolicy);
        }
        prewitt[isColored]->setXEnabled(ui->horizontalCheck->isChecked());
        prewitt[isColored]->setYEnabled(ui->verticalCheck->isChecked());
        mainWindow->setFilteredImage(prewitt[isColored]->getFilteredImage());

    } else if (ui->sobelButton->isChecked()) {
        if (isColored) {
            sobel[isColored] = new SobelOperator(
                        imageProcessor->getOriginImage(),
                        isColored, borderPolicy);
        } else {
            sobel[isColored] = new SobelOperator(
                        imageProcessor->getGrayScaleImage(),
                        isColored, borderPolicy);
        }
        sobel[isColored]->setXEnabled(ui->horizontalCheck->isChecked());
        sobel[isColored]->setYEnabled(ui->verticalCheck->isChecked());
        mainWindow->setFilteredImage(sobel[isColored]->getFilteredImage());
    }
}

void FilterDialog::on_nearestButton_clicked()
{
    borderPolicy = ImagePolicy::NEAREST;
}

void FilterDialog::on_mirrorButton_clicked()
{
    borderPolicy = ImagePolicy::MIRROR;
}

void FilterDialog::on_periodicButton_clicked()
{
    borderPolicy = ImagePolicy::PERIODIC;
}

void FilterDialog::on_blackButton_clicked()
{
    borderPolicy = ImagePolicy::BLACK;
}

void FilterDialog::on_horizontalCheck_clicked(bool checked)
{
    if (!checked && !ui->verticalCheck->isChecked()) {
        ui->verticalCheck->setChecked(true);
    }
}

void FilterDialog::on_verticalCheck_clicked(bool checked)
{
    if (!checked && !ui->horizontalCheck->isChecked()) {
        ui->horizontalCheck->setChecked(true);
    }
}

void FilterDialog::on_rgbCheck_clicked(bool checked)
{
    isColored = checked;
}
