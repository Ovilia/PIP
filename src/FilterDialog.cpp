#include <QSpinBox>

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
    isColored(true),
    customedPtr(0)
{
    ui->setupUi(this);

    for (int i = 0; i < 2; ++i) {
        prewitt[i] = 0;
        roberts[i] = 0;
        sobel[i] = 0;
        gaussian[i] = 0;
        mean[i] = 0;
        median[i] = 0;
    }
    customed = 0;

    ui->filterParaGroup->setVisible(false);
    ui->gaussianParaGroup->setVisible(false);
    ui->customedParaGroup->setVisible(false);

    for (int i = 0; i < MAX_KERNEL_RADIO; ++i) {
        for (int j = 0; j < MAX_KERNEL_RADIO; ++j) {
            int index = i * MAX_KERNEL_RADIO + j;
            kernelSpin[index] = new QSpinBox();
            kernelSpin[index]->setMaximum(255);
            kernelSpin[index]->setMinimum(-255);
            kernelSpin[index]->setValue(0);
            ui->gridLayout->addWidget(kernelSpin[index], i, j, 1, 1);
        }
    }
    resetCustEdit(MIN_KERNEL_RADIO);
}

FilterDialog::~FilterDialog()
{
    int amt = MAX_KERNEL_RADIO * MAX_KERNEL_RADIO;
    for (int i = 0; i < amt; ++i) {
        delete kernelSpin[i];
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
        if (gaussian[i]) {
            delete gaussian[i];
        }
        if (mean[i]) {
            delete mean[i];
        }
        if (median[i]) {
            delete median[i];
        }
    }

    if (customed) {
        delete customed;
    }
    if (customedPtr) {
        delete customedPtr;
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
        ui->filterParaGroup->setVisible(false);
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
                kernelSpin[i * MAX_KERNEL_RADIO + j]->setVisible(true);
            } else {
                kernelSpin[i * MAX_KERNEL_RADIO + j]->setVisible(false);
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
                            imageProcessor->getCurrentImage(),
                            isColored, borderPolicy);
            } else {
                roberts[isColored] = new RobertsOperator(
                            imageProcessor->getGrayScaleImage(),
                            isColored, borderPolicy);
            }
        } else {
            roberts[isColored]->setBorderPolicy(borderPolicy);
        }
        roberts[isColored]->setXEnabled(ui->horizontalCheck->isChecked());
        roberts[isColored]->setYEnabled(ui->verticalCheck->isChecked());
        mainWindow->setFilteredImage(roberts[isColored]->getFilteredImage());

    } else if (ui->prewittButton->isChecked()) {
        if (!prewitt[isColored]) {
            if (isColored) {
                prewitt[isColored] = new PrewittOperator(
                            imageProcessor->getCurrentImage(),
                            isColored, borderPolicy);
            } else {
                prewitt[isColored] = new PrewittOperator(
                            imageProcessor->getGrayScaleImage(),
                            isColored, borderPolicy);
            }
        } else {
            prewitt[isColored]->setBorderPolicy(borderPolicy);
        }
        prewitt[isColored]->setXEnabled(ui->horizontalCheck->isChecked());
        prewitt[isColored]->setYEnabled(ui->verticalCheck->isChecked());
        mainWindow->setFilteredImage(prewitt[isColored]->getFilteredImage());

    } else if (ui->sobelButton->isChecked()) {
        if (!sobel[isColored]) {
            if (isColored) {
                sobel[isColored] = new SobelOperator(
                            imageProcessor->getCurrentImage(),
                            isColored, borderPolicy);
            } else {
                sobel[isColored] = new SobelOperator(
                            imageProcessor->getGrayScaleImage(),
                            isColored, borderPolicy);
            }
        } else {
            sobel[isColored]->setBorderPolicy(borderPolicy);
        }
        sobel[isColored]->setXEnabled(ui->horizontalCheck->isChecked());
        sobel[isColored]->setYEnabled(ui->verticalCheck->isChecked());
        mainWindow->setFilteredImage(sobel[isColored]->getFilteredImage());

    } else if (ui->gaussianButton->isChecked()) {
        double sigma = ui->sigmaSpin->value();
        // compare radio rather than sigma to avoid floating number comparation
        int radio = sigma * GaussianFilter::RADIO_DIVIDED_BY_SIGMA;
        if (!gaussian[isColored] ||
                radio != gaussian[isColored]->getKernelRadio()) {
            // gaussian kernel radio changed
            if (gaussian[isColored]) {
                delete gaussian[isColored];
            }
            if (isColored) {
                gaussian[isColored] = new GaussianFilter(
                            imageProcessor->getCurrentImage(),
                            sigma, isColored, borderPolicy);
            } else {
                gaussian[isColored] = new GaussianFilter(
                            imageProcessor->getGrayScaleImage(),
                            sigma, isColored, borderPolicy);
            }
        } else {
            gaussian[isColored]->setBorderPolicy(borderPolicy);
        }
        mainWindow->setFilteredImage(gaussian[isColored]->getFilteredImage());

    } else if (ui->meanButton->isChecked()) {
        int radio = ui->filterRadioSpin->value() / 2;
        if (!mean[isColored] ||
                radio != mean[isColored]->getKernelRadio()) {
            if (mean[isColored]) {
                delete mean[isColored];
            }
            if (isColored) {
                mean[isColored] = new MeanFilter(
                            imageProcessor->getCurrentImage(),
                            radio, isColored, borderPolicy);
            } else {
                mean[isColored] = new MeanFilter(
                            imageProcessor->getGrayScaleImage(),
                            radio, isColored, borderPolicy);
            }
        } else {
            mean[isColored]->setBorderPolicy(borderPolicy);
        }
        mainWindow->setFilteredImage(mean[isColored]->getFilteredImage());

    } else if (ui->medianFilter->isChecked()) {
        int radio = ui->filterRadioSpin->value() / 2;
        if (!median[isColored] ||
                radio != median[isColored]->getKernelRadio()) {
            if (median[isColored]) {
                delete median[isColored];
            }
            if (isColored) {
                median[isColored] = new MedianFilter(
                            imageProcessor->getCurrentImage(),
                            radio, isColored, borderPolicy);
            } else {
                median[isColored] = new MedianFilter(
                            imageProcessor->getGrayScaleImage(),
                            radio, isColored, borderPolicy);
            }
        } else {
            median[isColored]->setBorderPolicy(borderPolicy);
        }
        mainWindow->setFilteredImage(median[isColored]->getFilteredImage());

    } else if (ui->customedButton->isChecked()) {
        int spin = ui->filterRadioSpin->value();
        int radio = spin / 2;
        // customed kernel is usually not the same with the former ones
        // so it is not buffered
        if (customed) {
            delete customed;
        }
        if (isColored) {
            customed = new LinearFilter(imageProcessor->getCurrentImage(),
                                        radio, isColored, borderPolicy);
        } else {
            customed = new LinearFilter(imageProcessor->getGrayScaleImage(),
                                        radio, isColored, borderPolicy);
        }
        if (customedPtr) {
            delete customedPtr;
        }
        int length = (2 * radio + 1) * (2 * radio + 1);
        customedPtr = new int[length];
        int index = 0;
        for (int i = 0; i < spin; ++i) {
            for (int j = 0; j < spin; ++j) {
                customedPtr[index] = kernelSpin[MAX_KERNEL_RADIO * i + j]->value();
                ++index;
            }
        }
        customed->changeKernel(customedPtr);
        mainWindow->setFilteredImage(customed->getFilteredImage());
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
