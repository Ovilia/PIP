#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <Qlist>

namespace Ui {
    class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = 0);
    ~FilterDialog();

private slots:
    void on_robertsButton_clicked(bool checked);

    void on_prewittButton_clicked(bool checked);

    void on_sobelButton_clicked(bool checked);

    void on_gaussianButton_clicked(bool checked);

    void on_meanButton_clicked(bool checked);

    void on_medianFilter_clicked(bool checked);

    void on_customedButton_clicked(bool checked);

private:
    Ui::FilterDialog *ui;

    void disableAllButtons();
    void resetCustEdit(int count);
};

#endif // FILTERDIALOG_H
