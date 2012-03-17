#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

#include "ImageProcessor.h"
#include "ImageWidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionGray_Scale_triggered();

    void on_actionHistogram_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget* tabWidget;
    ImageWidget* originWidget;
    ImageWidget* grayScaleWidget;

    // file location of image
    QString imagePath;
    ImageProcessor* imageProcessor;

    // if is the first time opening an image
    bool isFirstImage;
};

#endif // MAINWINDOW_H
