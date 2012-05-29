#include "MorphologyDialog.h"
#include "ui_MorphologyDialog.h"

#include <qmath.h>

#include <QDebug>

MorphologyDialog::MorphologyDialog(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MorphologyDialog),
    mainWindow(mainWindow),
    binaryMorpho(0),
    grayMorpho(0),
    seArr(0),
    seSize(0),
    seType(ST_SQUARE)
{
    ui->setupUi(this);

    ui->seWidget->setVisible(false);
    ui->sizeSpin->setMaximum(MAX_SE_RADIO);
    ui->sizeSpin->setValue(MIN_SE_RADIO);
    resetSeArr(MIN_SE_RADIO, seType);

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
                seSpin[i * MAX_CUSTOM_SE_RADIO + j]->setVisible(true);
            } else {
                seSpin[i * MAX_CUSTOM_SE_RADIO + j]->setVisible(false);
            }
        }
    }
    resize(minimumSize());
}

void MorphologyDialog::resetSeArr(int radius, SeType seType)
{
    int length = radius * radius;
    if (seSize != length) {
        if (seArr) {
            delete seArr;
        }
        seArr = new int[length];
        seSize = length;
    } else if (seType == this->seType) {
        // se is the same as the previous one
        return;
    }
    switch (seType) {
    case ST_CUSTOMED:
        // all 0
        for (int i = 0; i < seSize; ++i) {
            seArr[i] = 0;
        }
        break;

    case ST_SQUARE:
        // all 1
        for (int i = 0; i < seSize; ++i) {
            seArr[i] = 1;
        }
        break;

    case ST_CIRCLE:
        for (int w = 0; w < radius; ++w) {
            for (int h = 0; h < radius; ++h) {
                if ((w - radius / 2) * (w - radius / 2) +
                        (h - radius / 2) * (h - radius / 2)
                        < length / 4) {
                    // within a circle
                    seArr[w + h * radius] = 1;
                    qDebug() << w + h * radius << 1;
                } else {
                    seArr[w + h * radius] = 0;
                    qDebug() << w + h * radius << 0;
                }
            }
        }
        break;

    case ST_CROSS:
    {
        for (int i = 0; i < seSize; ++i) {
            seArr[i] = 0;
        }
        int center = radius / 2;
        for (int i = 0; i < radius; ++i) {
            seArr[center * radius + i] = 1;
            seArr[center + i * radius] = 1;
        }
    }
        break;

    case ST_DIAMOND:
    {
        int center = radius / 2;
        for (int i = 0; i < radius; ++i) {
            int start = qAbs(i - center);
            int end = center * 2 - start;
            for (int j = start; j <= end; ++j) {
                seArr[i * radius + j] = 1;
            }
        }
    }
        break;
    }
    this->seType = seType;
}

void MorphologyDialog::on_custButton_clicked()
{
    ui->seWidget->setVisible(true);
    ui->sizeSpin->setMaximum(MAX_CUSTOM_SE_RADIO);
    seType = ST_CUSTOMED;
    ui->sizeSpin->setValue(MIN_SE_RADIO);
    resetCustSpin(MIN_SE_RADIO);
}

void MorphologyDialog::on_squareButton_clicked()
{
    ui->seWidget->setVisible(false);
    ui->sizeSpin->setMaximum(MAX_SE_RADIO);
    seType = ST_SQUARE;
    resize(minimumSize());
}

void MorphologyDialog::on_circleButton_clicked()
{
    ui->seWidget->setVisible(false);
    ui->sizeSpin->setMaximum(MAX_SE_RADIO);
    seType = ST_CIRCLE;
    resize(minimumSize());
}

void MorphologyDialog::on_crossButton_clicked()
{
    ui->seWidget->setVisible(false);
    ui->sizeSpin->setMaximum(MAX_SE_RADIO);
    seType = ST_CROSS;
    resize(minimumSize());
}

void MorphologyDialog::on_diamondButton_clicked()
{
    ui->seWidget->setVisible(false);
    ui->sizeSpin->setMaximum(MAX_SE_RADIO);
    seType = ST_DIAMOND;
    resize(minimumSize());
}

void MorphologyDialog::on_applyButton_clicked()
{
    int radius = ui->sizeSpin->value();
    if (ui->custButton->isChecked()) {
        // TODO: custom se set
    } else {
        resetSeArr(radius, seType);
    }
    StructElement se(radius, radius, radius / 2, radius / 2, seArr);

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
    if (seType != ST_CUSTOMED) {
        // customed spin check is in on_sizeSpin_valueChanged
        int origin = ui->sizeSpin->value();
        int value = origin / 2 * 2 + 1;
        if (value != origin) {
            // invalid input se size
            ui->sizeSpin->setValue(value);
        }
    }
}

void MorphologyDialog::on_sizeSpin_valueChanged(int arg1)
{
    if (seType == ST_CUSTOMED) {
        int value = arg1 / 2 * 2 + 1;
        if (value != arg1) {
            // invalid input se size
            ui->sizeSpin->setValue(value);
        }
        resetCustSpin(value);
    }
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
