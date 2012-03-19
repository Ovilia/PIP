#include "ImageWidget.h"
#include "ui_ImageWidget.h"

ImageWidget::ImageWidget(QPixmap pixmap, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget)
{
    ui->setupUi(this);
    this->pixmap = pixmap;
    ui->imageLabel->setPixmap(pixmap);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::setPixmap(QPixmap pixmap)
{
    this->pixmap = pixmap;
    ui->imageLabel->setPixmap(pixmap);
}
