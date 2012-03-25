#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

#include "FilterDialog.h"
#include "HistogramDialog.h"
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

    void repaintBinary();
    void setFilteredImage(QImage *image);

private slots:
    void on_actionOpen_triggered();

    void on_actionHistogram_triggered();

    void on_actionFilter_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget* tabWidget;
    ImageWidget* originWidget;
    ImageWidget* binaryWidget;
    ImageWidget* filteredWidget;

    class HistogramDialog* histogramDialog;
    class FilterDialog* filterDialog;

    // file location of image
    QString imagePath;
    ImageProcessor* imageProcessor;

    // if is the first time opening an image
    bool isFirstImage;
};

#endif // MAINWINDOW_H
