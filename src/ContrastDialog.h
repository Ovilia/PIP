#ifndef CONTRASTDIALOG_H
#define CONTRASTDIALOG_H

#include <QDialog>

#include "MainWindow.h"

namespace Ui {
    class ContrastDialog;
}

class ContrastDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContrastDialog(class MainWindow* mainWindow,
                            QWidget *parent = 0);
    ~ContrastDialog();

private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::ContrastDialog *ui;
    MainWindow* mainWindow;
};

#endif // CONTRASTDIALOG_H
