#include <QMouseEvent>
#include <QEvent>

#include "ImageLabel.h"

ImageLabel::ImageLabel(QImage* image, ImageWidget* imageWidget,
                       QWidget *parent) :
    QLabel(parent),
    image(image),
    imageWidget(imageWidget)
{
}

void ImageLabel::enterEvent(QEvent *)
{
    setMouseTracking(true);
}

void ImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (imageWidget) {
        imageWidget->setMousePosition(event->x(), event->y());
    }
}
