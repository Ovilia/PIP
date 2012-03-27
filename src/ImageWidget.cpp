#include "ImageWidget.h"
#include "ui_ImageWidget.h"

ImageWidget::ImageWidget(QImage* image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget),
    image(0)
{
    ui->setupUi(this);
    imageLabel = new ImageLabel(image, this, this);
    imageLabel->setMinimumSize(
                image->size().width(), image->size().height());
    imageLabel->setMaximumSize(
                image->size().width(), image->size().height());
    ui->scrollArea->setWidget(imageLabel);
    setImage(image);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::setImage(QImage* image)
{
    this->image = image;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
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
    imageLabel->setMinimumSize(width, height);
    imageLabel->setMaximumSize(width, height);
}

void ImageWidget::setMousePosition(int x, int y)
{
    ui->xPosLabel->setText(QString::number(x));
    ui->yPosLabel->setText(QString::number(y));
    const uchar* bits = image->constBits() +
            (y * image->width() + x) * PIXEL_SIZE;
    ui->rLabel->setText(QString::number(*(bits + 2)));
    ui->gLabel->setText(QString::number(*(bits + 1)));
    ui->bLabel->setText(QString::number(*bits));
}
