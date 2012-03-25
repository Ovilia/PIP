#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QLineEdit>

#include "ImagePolicy.h"
#include "ImageProcessor.h"
#include "MainWindow.h"
#include "PrewittOperator.h"
#include "RobertsOperator.h"
#include "SobelOperator.h"

namespace Ui {
    class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(class MainWindow* mainWindow,
                          ImageProcessor* imageProcessor,
                          QWidget *parent = 0);
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

    void on_nearestButton_clicked();

    void on_mirrorButton_clicked();

    void on_periodicButton_clicked();

    void on_blackButton_clicked();

private:
    Ui::FilterDialog *ui;
    MainWindow* mainWindow;
    ImageProcessor* imageProcessor;

    static const int MIN_KERNEL_RADIO = 3;
    static const int MAX_KERNEL_RADIO = 9;
    QLineEdit* kernelEdit[MAX_KERNEL_RADIO * MAX_KERNEL_RADIO];

    ImagePolicy::BorderPolicy borderPolicy;
    PrewittOperator* prewitt;
    RobertsOperator* roberts;
    SobelOperator* sobel;

    void disableAllButtons();
    void resetCustEdit(int count);
};

#endif // FILTERDIALOG_H
