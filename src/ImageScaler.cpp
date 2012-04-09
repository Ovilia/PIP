#include "ImageScaler.h"

ImageScaler::ImageScaler()
{
}

QImage* ImageScaler::getScaledImage(QImage* originImage,
                                    int newWidth, int newHeight,
                                    ImagePolicy::ScalePolicy policy)
{
    switch (policy) {
    case ImagePolicy::SP_NEAREST:
        return getNearestImage(originImage, newWidth, newHeight);

    case ImagePolicy::SP_BILINEAR:
        return getBilinearImage(originImage, newWidth, newHeight);
    }
    return 0;
}

QImage* ImageScaler::getNearestImage(QImage* originImage,
                                     int newWidth, int newHeight)
{
    QImage* newImage = new QImage(newWidth, newHeight, originImage->format());
    uchar* newBits = newImage->bits();
    const uchar* oBits = originImage->constBits();

    int oWidth = originImage->width();
    int oHeight = originImage->height();
    for (int i = 0; i < newHeight; ++i) {
        for (int j = 0; j < newWidth; ++j) {
            int x = j * oWidth / newWidth;
            int y = i * oHeight / newHeight;
            int offset = oBits + (y * oWidth + x) * 4;
            for (int rgb = 0; rgb < 3; ++rgb) {
                *newBits = *(offset + rgb);
                ++newBits;
            }
            ++newBits;
        }
    }

    return newImage;
}

QImage* ImageScaler::getBilinearImage(QImage* originImage,
                                      int newWidth, int newHeight)
{
    QImage* newImage = new QImage(newWidth, newHeight, originImage->format());
    uchar* newBits = newImage->bits();
    const uchar* oBits = originImage->constBits();

    int oWidth = originImage->width();
    int oHeight = originImage->height();
    for (int i = 0; i < newHeight; ++i) {
        for (int j = 0; j < newWidth; ++j) {
            int x0 = j * oWidth / newWidth;
            int y0 = i * oHeight / newHeight;
            int x1 = x0 + 1;
            int y1 = y0 + 1;
        }
    }

    return newImage;
}
