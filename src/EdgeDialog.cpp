#include "EdgeDialog.h"
#include "ui_EdgeDialog.h"

EdgeDialog::EdgeDialog(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EdgeDialog),
    mainWindow(mainWindow)
{
    ui->setupUi(this);
}

EdgeDialog::~EdgeDialog()
{
    delete ui;
}
