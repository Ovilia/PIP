#ifndef EDGEDIALOG_H
#define EDGEDIALOG_H

#include <QDialog>

#include "MainWindow.h"

namespace Ui {
    class EdgeDialog;
}

class EdgeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EdgeDialog(MainWindow* mainWindow, QWidget *parent = 0);
    ~EdgeDialog();

private:
    Ui::EdgeDialog *ui;

    MainWindow* mainWindow;
};

#endif // EDGEDIALOG_H
