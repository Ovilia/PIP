#include "ImageWidget.h"
#include "ui_ImageWidget.h"

ImageWidget::ImageWidget(QImage* image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget),
    image(0)
{
    ui->setupUi(this);
    setImage(image);
    ui->imageLabel->setMinimumSize(
                image->size().width(), image->size().height());
    ui->imageLabel->setMaximumSize(
                image->size().width(), image->size().height());
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::setImage(QImage* image)
{
    this->image = image;
    ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
    ui->scaleLabel->setText("100%");
    ui->horizontalSlider->setValue(100);
}

void ImageWidget::on_horizontalSlider_valueChanged(int value)
{
    int width, height;
    if (value == 0) {
        width = 1;
        height = 1;
    } else {
        width = image->size().width() * value / 100;
        height = image->size().height() * value / 100;
    }
    ui->scaleLabel->setText(QString::number(value) + "%");
    ui->imageLabel->setMinimumSize(width, height);
    ui->imageLabel->setMaximumSize(width, height);
}
