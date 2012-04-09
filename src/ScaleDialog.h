#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QDialog>
#include <QImage>

#include "MainWindow.h"

namespace Ui {
    class ScaleDialog;
}

class ScaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScaleDialog(MainWindow* mainWindow, QWidget *parent = 0);
    ~ScaleDialog();

private slots:
    void on_resetButton_clicked();

    void on_applyButton_clicked();

private:
    Ui::ScaleDialog *ui;
    MainWindow* mainWindow;

    QImage* originImage;
};

#endif // SCALEDIALOG_H
