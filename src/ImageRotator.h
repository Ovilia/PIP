#ifndef IMAGEROTATOR_H
#define IMAGEROTATOR_H

#include <QImage>

#include "ImagePolicy.h"

class ImageRotator
{
public:
    static QImage* getRotatedImage(QImage *old, int angle);

private:
    ImageRotator();
};

#endif // IMAGEROTATOR_H
