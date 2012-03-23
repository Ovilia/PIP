#include <QLineEdit>
#include <QList>

#include "FilterDialog.h"
#include "ui_FilterDialog.h"

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

FilterDialog::~FilterDialog()
{
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
}

void FilterDialog::on_robertsButton_clicked(bool checked)
{
    disableAllButtons();
    ui->robertsButton->setChecked(true);
    ui->filterParaGroup->setEnabled(false);
}

void FilterDialog::on_prewittButton_clicked(bool checked)
{
    disableAllButtons();
    ui->prewittButton->setChecked(true);
    ui->filterParaGroup->setEnabled(false);
}

void FilterDialog::on_sobelButton_clicked(bool checked)
{
    disableAllButtons();
    ui->sobelButton->setChecked(true);
    ui->filterParaGroup->setEnabled(false);
}

void FilterDialog::on_gaussianButton_clicked(bool checked)
{
    disableAllButtons();
    ui->gaussianButton->setChecked(true);
    ui->filterParaGroup->setEnabled(true);
}

void FilterDialog::on_meanButton_clicked(bool checked)
{
    disableAllButtons();
    ui->meanButton->setChecked(true);
    ui->filterParaGroup->setEnabled(true);
}

void FilterDialog::on_medianFilter_clicked(bool checked)
{
    disableAllButtons();
    ui->medianFilter->setChecked(true);
    ui->filterParaGroup->setEnabled(true);
}

void FilterDialog::on_customedButton_clicked(bool checked)
{
    disableAllButtons();
    ui->customedButton->setChecked(true);
    ui->filterParaGroup->setEnabled(true);
    ui->customedParaGroup->setEnabled(true);
}

void FilterDialog::resetCustEdit(int count)
{
    ui->tableWidget->setColumnCount(count);
    ui->tableWidget->setRowCount(count);
}
