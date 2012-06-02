#include <QFileDialog>

#include "ImageWidget.h"
#include "ui_ImageWidget.h"

ImageWidget::ImageWidget(MainWindow* mainWindow,
                         QImage* image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget),
    image(0),
    mainWindow(mainWindow)
{
    ui->setupUi(this);
    imageLabel = new ImageLabel(image, this, this);
    imageLabel->setMinimumSize(
                image->size().width(), image->size().height());
    imageLabel->setMaximumSize(
                image->size().width(), image->size().height());
    imageLabel->setScaledContents(true);
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
    imageLabel->setMinimumSize(
                image->size().width(), image->size().height());
    imageLabel->setMaximumSize(
                image->size().width(), image->size().height());
    ui->scaleLabel->setText("100%");
    ui->horizontalSlider->setValue(100);
}

QImage* ImageWidget::getImage()
{
    return image;
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
    double ratio = ui->horizontalSlider->value() / 100.0;
    int xInt = (int)(x / ratio);
    int yInt = (int)(y / ratio);

    ui->xPosLabel->setText(QString::number(xInt));
    ui->yPosLabel->setText(QString::number(yInt));
    const uchar* bits = image->constBits() +
            (yInt * image->width() + xInt) * PIXEL_SIZE;
    ui->rLabel->setText(QString::number(*(bits + 2)));
    ui->gLabel->setText(QString::number(*(bits + 1)));
    ui->bLabel->setText(QString::number(*bits));
}

void ImageWidget::on_saveButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(
                this, tr("Save an Image"), QDir::currentPath(),
                tr("Image files(*.bmp *.jpeg *.jpg *.png *.gif);;All files (*.*)"));
    if (!filePath.isNull()) {
        image->save(filePath);
    }
}

void ImageWidget::on_applyButton_clicked()
{
    mainWindow->resetImage(image);
}
