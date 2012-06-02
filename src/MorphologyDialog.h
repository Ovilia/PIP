#ifndef MORPHOLOGYDIALOG_H
#define MORPHOLOGYDIALOG_H

#include <QDialog>
#include <QSpinBox>

#include "MainWindow.h"
#include "StructElement.h"

namespace Ui {
    class MorphologyDialog;
}

class MorphologyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MorphologyDialog(MainWindow* mainWindow, QWidget *parent = 0);
    ~MorphologyDialog();

private slots:
    void on_custButton_clicked();

    void on_squareButton_clicked();

    void on_circleButton_clicked();

    void on_crossButton_clicked();

    void on_diamondButton_clicked();

    void on_applyButton_clicked();

    void on_undoButton_clicked();

    void on_redoButton_clicked();

    void on_sizeSpin_editingFinished();

    void on_sizeSpin_valueChanged(int arg1);

    void on_grayButton_clicked();

    void on_binaryButton_clicked();

private:
    Ui::MorphologyDialog *ui;
    MainWindow* mainWindow;

    BinaryMorphology* binaryMorpho;
    GrayMorphology* grayMorpho;

    static const int MIN_SE_RADIO = 3;
    static const int MAX_SE_RADIO = 255;
    static const int MAX_CUSTOM_SE_RADIO = 7;
    QSpinBox* seSpin[MAX_CUSTOM_SE_RADIO * MAX_CUSTOM_SE_RADIO];
    void resetCustSpin(int radius);

    int* seArr;
    int seRadius;
    StructElement::SE_Type seType;
};

#endif // MORPHOLOGYDIALOG_H
