#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QPixmap>
#include <QWidget>

#include "ImageLabel.h"

namespace Ui {
    class ImageWidget;
}

class ImageLabel;

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QImage* image, QWidget *parent = 0);
    ~ImageWidget();

    void setImage(QImage* image);

    // called by ImageLabel to update rgb info
    void setMousePosition(int x, int y);

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_saveButton_clicked();

private:
    static const int PIXEL_SIZE =4;
    Ui::ImageWidget *ui;
    ImageLabel* imageLabel;
    QImage* image;
};

#endif // IMAGEWIDGET_H
