#ifndef ALGEBRADIALOG_H
#define ALGEBRADIALOG_H

#include <QDialog>
#include <QImage>

#include "MainWindow.h"

namespace Ui {
    class AlgebraDialog;
}

class AlgebraDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlgebraDialog(class MainWindow* mainWindow, QWidget *parent = 0);
    ~AlgebraDialog();

private slots:
    void on_openButton_clicked();

    void on_applyButton_clicked();

private:
    Ui::AlgebraDialog *ui;
    MainWindow* mainWindow;

    QImage* maskImage;
};

#endif // ALGEBRADIALOG_H
