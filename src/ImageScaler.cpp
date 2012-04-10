#include <QDebug>

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
            const uchar* offset = oBits + (y * oWidth + x) * 4;
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
    const uchar* const oBits = originImage->constBits();

    int oWidth = originImage->width();
    int oHeight = originImage->height();
    for (int i = 0; i < newHeight; ++i) {
        for (int j = 0; j < newWidth; ++j) {
            float x = (float)j * oWidth / newWidth;
            float y = (float)i * oHeight / newHeight;
            int x1 = (int)x;
            int y1 = (int)y;
            const uchar* const q11 = oBits + (y1 * oWidth + x1) * 4;
            // check if is the end of a line, if so, don't use the next line
            const uchar* const q21 = (j == newWidth - 1) ? q11 : (q11 + 4);
            const uchar* const q12 = (i == newHeight - 1) ? q11 : (q11 + oWidth * 4);
            const uchar* const q22 = (j == newWidth - 1) ? q12 : (q12 + 4);
            bool a = j == newWidth - 1;
            bool b = i == newHeight - 1;
            //qDebug()<<*q11<<*q21<<*q12<<*q22<<"\t("<<a<<b<<")";
            for (int rgb = 0; rgb < 3; ++rgb) {
                *newBits = (uchar)(*(q11 + rgb) * (x1 + 1 - x) * (y1 + 1 - y) +
                                   *(q21 + rgb) * (x - x1) * (y1 + 1 - y) +
                                   *(q12 + rgb) * (x1 + 1 - x) * (y - y1) +
                                   *(q22 + rgb) * (x - x1) * (y - y1));
                ++newBits;
            }
            ++newBits;
        }
    }

    return newImage;
}
