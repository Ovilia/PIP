#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QPixmap>
#include <QWidget>

namespace Ui {
    class ImageWidget;
}

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QImage* image, QWidget *parent = 0);
    ~ImageWidget();

    void setImage(QImage* image);

private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::ImageWidget *ui;
    QImage* image;
};

#endif // IMAGEWIDGET_H
