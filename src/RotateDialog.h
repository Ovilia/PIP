#ifndef ROTATEDIALOG_H
#define ROTATEDIALOG_H

#include <QDialog>

#include "MainWindow.h"

namespace Ui {
    class RotateDialog;
}

class RotateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RotateDialog(MainWindow* mainWindow, QWidget *parent = 0);
    ~RotateDialog();

private slots:
    void on_angleSlider_sliderMoved(int position);

    void on_angleSlider_sliderReleased();

private:
    Ui::RotateDialog *ui;
    MainWindow* mainWindow;
};

#endif // ROTATEDIALOG_H
