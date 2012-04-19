#ifndef BRIGHTDIALOG_H
#define BRIGHTDIALOG_H

#include <QDialog>

#include "MainWindow.h"

namespace Ui {
    class BrightDialog;
}

class BrightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrightDialog(class MainWindow* mainWindow, QWidget *parent = 0);
    ~BrightDialog();

private slots:
    void on_brightSlider_valueChanged(int value);

private:
    Ui::BrightDialog *ui;
    MainWindow* mainWindow;
};

#endif // BRIGHTDIALOG_H
