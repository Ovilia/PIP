#include "MorphologyDialog.h"
#include "ui_MorphologyDialog.h"

#include <qmath.h>

MorphologyDialog::MorphologyDialog(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MorphologyDialog),
    mainWindow(mainWindow),
    binaryMorpho(0),
    grayMorpho(0),
    seArr(0),
    seRadius(MIN_SE_RADIO),
    seType(StructElement::ST_SQUARE)
{
    ui->setupUi(this);

    ui->seWidget->setVisible(false);
    ui->sizeSpin->setMaximum(MAX_SE_RADIO);
    ui->sizeSpin->setValue(MIN_SE_RADIO);

    for (int i = 0; i < MAX_CUSTOM_SE_RADIO; ++i) {
        for (int j = 0; j < MAX_CUSTOM_SE_RADIO; ++j) {
            int index = i * MAX_CUSTOM_SE_RADIO + j;
            seSpin[index] = new QSpinBox();
            seSpin[index]->setMaximum(255);
            seSpin[index]->setMinimum(-255);
            seSpin[index]->setValue(1);
            ui->seLayout->addWidget(seSpin[index], i, j, 1, 1);
        }
    }
}

MorphologyDialog::~MorphologyDialog()
{
    if (seArr) {
        delete []seArr;
    }

    delete ui;
}

void MorphologyDialog::resetCustSpin(int radius)
{
    for (int i = 0; i < MAX_CUSTOM_SE_RADIO; ++i) {
        for (int j = 0; j < MAX_CUSTOM_SE_RADIO; ++j) {
            if (i < radius && j < radius) {
                seSpin[i * MAX_CUSTOM_SE_RADIO + j]->setEnabled(true);
            } else {
                seSpin[i * MAX_CUSTOM_SE_RADIO + j]->setEnabled(false);
            }
        }
    }
    resize(minimumSize());
}

void MorphologyDialog::on_custButton_clicked()
{
    ui->seWidget->setVisible(true);
    ui->sizeSpin->setMaximum(MAX_CUSTOM_SE_RADIO);
    seType = StructElement::ST_CUSTOMED;
    ui->sizeSpin->setValue(MIN_SE_RADIO);
    resetCustSpin(MIN_SE_RADIO);
}

void MorphologyDialog::on_squareButton_clicked()
{
    ui->seWidget->setVisible(false);
    ui->sizeSpin->setMaximum(MAX_SE_RADIO);
    seType = StructElement::ST_SQUARE;
    resize(minimumSize());
}

void MorphologyDialog::on_circleButton_clicked()
{
    ui->seWidget->setVisible(false);
    ui->sizeSpin->setMaximum(MAX_SE_RADIO);
    seType = StructElement::ST_CIRCLE;
    resize(minimumSize());
}

void MorphologyDialog::on_crossButton_clicked()
{
    ui->seWidget->setVisible(false);
    ui->sizeSpin->setMaximum(MAX_SE_RADIO);
    seType = StructElement::ST_CROSS;
    resize(minimumSize());
}

void MorphologyDialog::on_diamondButton_clicked()
{
    ui->seWidget->setVisible(false);
    ui->sizeSpin->setMaximum(MAX_SE_RADIO);
    seType = StructElement::ST_DIAMOND;
    resize(minimumSize());
}

void MorphologyDialog::on_applyButton_clicked()
{
    if (ui->custButton->isChecked()) {
        if (seArr) {
            delete seArr;
        }
        seArr = new int[seRadius * seRadius];
        int loops = MAX_CUSTOM_SE_RADIO * MAX_CUSTOM_SE_RADIO;
        int index = 0;
        for (int i = 0; i < loops; ++i) {
            if (seSpin[i]->isEnabled()) {
                seArr[index] = seSpin[i]->value();
                ++index;
            }
        }
    }
    StructElement se(seRadius, seType, seArr);

    Morphology* morpho;
    if (ui->binaryButton->isChecked()) {
        if (!binaryMorpho) {
            binaryMorpho = mainWindow->getBinaryMorpo();
        }
        binaryMorpho->setForeground(ui->whiteCheck->isChecked());
        morpho = binaryMorpho;
    } else {
        if (!grayMorpho) {
            grayMorpho = mainWindow->getGrayMorpo();
        }
        morpho = grayMorpho;
    }

    if (ui->dilationButton->isChecked()) {
        mainWindow->setMorphologyImage(morpho->doDilation(se));
    } else if (ui->erosionButton->isChecked()) {
        mainWindow->setMorphologyImage(morpho->doErosion(se));
    } else if (ui->openButton->isChecked()) {
        mainWindow->setMorphologyImage(morpho->doOpening(se));
    } else if (ui->closingButton->isChecked()) {
        mainWindow->setMorphologyImage(morpho->doClosing(se));
    }

    ui->redoButton->setEnabled(morpho->canRedo());
    ui->undoButton->setEnabled(morpho->canUndo());
}

void MorphologyDialog::on_undoButton_clicked()
{
    Morphology* morpho;
    if (ui->binaryButton->isChecked()) {
        morpho = binaryMorpho;
    } else {
        morpho = grayMorpho;
    }
    if (morpho->undo()) {
        // undo successfully
        mainWindow->setMorphologyImage(morpho->getOperatedImage());
    }
    ui->redoButton->setEnabled(morpho->canRedo());
    ui->undoButton->setEnabled(morpho->canUndo());
}

void MorphologyDialog::on_redoButton_clicked()
{
    Morphology* morpho;
    if (ui->binaryButton->isChecked()) {
        morpho = binaryMorpho;
    } else {
        morpho = grayMorpho;
    }
    if (morpho->redo()) {
        // redo successfully
        mainWindow->setMorphologyImage(morpho->getOperatedImage());
    }
    ui->redoButton->setEnabled(morpho->canRedo());
    ui->undoButton->setEnabled(morpho->canUndo());
}

void MorphologyDialog::on_sizeSpin_editingFinished()
{
    if (seType != StructElement::ST_CUSTOMED) {
        // customed spin check is in on_sizeSpin_valueChanged
        int origin = ui->sizeSpin->value();
        int value = origin / 2 * 2 + 1;
        if (value != origin) {
            // invalid input se size
            ui->sizeSpin->setValue(value);
        }
    }
    seRadius = ui->sizeSpin->value();
}

void MorphologyDialog::on_sizeSpin_valueChanged(int arg1)
{
    if (seType == StructElement::ST_CUSTOMED) {
        int value = arg1 / 2 * 2 + 1;
        if (value != arg1) {
            // invalid input se size
            ui->sizeSpin->setValue(value);
        }
        resetCustSpin(value);
    }
    seRadius = ui->sizeSpin->value();
}

void MorphologyDialog::on_grayButton_clicked()
{
    if (grayMorpho) {
        ui->undoButton->setEnabled(grayMorpho->canUndo());
        ui->redoButton->setEnabled(grayMorpho->canRedo());
    } else {
        ui->undoButton->setEnabled(false);
        ui->redoButton->setEnabled(false);
    }
    ui->whiteCheck->setEnabled(false);
}

void MorphologyDialog::on_binaryButton_clicked()
{
    if (binaryMorpho) {
        ui->undoButton->setEnabled(binaryMorpho->canUndo());
        ui->redoButton->setEnabled(binaryMorpho->canRedo());
    } else {
        ui->undoButton->setEnabled(false);
        ui->redoButton->setEnabled(false);
    }
    ui->whiteCheck->setEnabled(true);
}
