#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QLineEdit>

#include "MainWindow.h"

namespace Ui {
    class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(class MainWindow* mainWindow, QWidget *parent = 0);
    ~FilterDialog();

private slots:
    void on_robertsButton_clicked(bool checked);

    void on_prewittButton_clicked(bool checked);

    void on_sobelButton_clicked(bool checked);

    void on_gaussianButton_clicked(bool checked);

    void on_meanButton_clicked(bool checked);

    void on_medianFilter_clicked(bool checked);

    void on_customedButton_clicked(bool checked);

    void on_filterRadioSpin_valueChanged(int value);

    void on_applyButton_clicked();

private:
    Ui::FilterDialog *ui;
    MainWindow* mainWindow;

    static const int MIN_KERNEL_RADIO = 3;
    static const int MAX_KERNEL_RADIO = 9;
    QLineEdit* kernelEdit[MAX_KERNEL_RADIO * MAX_KERNEL_RADIO];

    void disableAllButtons();
    void resetCustEdit(int count);
};

#endif // FILTERDIALOG_H
