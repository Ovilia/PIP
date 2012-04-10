#include "Algebra.h"
#include "ImageScaler.h"

QImage* Algebra::resultImage = 0;
QImage* Algebra::scaledImage = 0;

Algebra::Algebra()
{
}

QImage* Algebra::getAlgebraImage(QImage* baseImage, QImage* maskImage,
                                   ImagePolicy::AlgebraPolicy algebraPolicy,
                                   ImagePolicy::ScalePolicy scalePolicy)
{
    int width = baseImage->width();
    int height = baseImage->height();
    scaledImage = ImageScaler::getScaledImage(
                maskImage, width, height, scalePolicy);
    resultImage = new QImage(width, height, baseImage->format());

    switch (algebraPolicy) {
    case ImagePolicy::AP_ADD:
        return getAddImage(baseImage);

    case ImagePolicy::AP_MINUS:
        return getMinusImage(baseImage);

    case ImagePolicy::AP_MULTIPLY:
        return getMultiplyImage(baseImage);

    case ImagePolicy::AP_DIVIDE:
        return getDivideImage(baseImage);
    }
    return 0;
}

QImage* Algebra::getAddImage(QImage* baseImage)
{
    int size = baseImage->width() * baseImage->height();
    const uchar* bBits = baseImage->constBits();
    const uchar* mBits = scaledImage->constBits();
    uchar* rBits = resultImage->bits();

    for (int i = 0; i < size; ++i) {
        for (int rgb = 0; rgb < 3; ++rgb) {
            int value = (int)*bBits + *mBits;
            if (value > 255) {
                value = 255;
            }
            *rBits = value;

            ++bBits;
            ++mBits;
            ++rBits;
        }
        ++bBits;
        ++mBits;
        ++rBits;
    }
    delete scaledImage;
    return resultImage;
}

QImage* Algebra::getMinusImage(QImage* baseImage)
{
    int size = baseImage->width() * baseImage->height();
    const uchar* bBits = baseImage->constBits();
    const uchar* mBits = scaledImage->constBits();
    uchar* rBits = resultImage->bits();

    for (int i = 0; i < size; ++i) {
        for (int rgb = 0; rgb < 3; ++rgb) {
            int value = (int)*bBits - *mBits;
            if (value < 0) {
                value = 0;
            }
            *rBits = value;

            ++bBits;
            ++mBits;
            ++rBits;
        }
        ++bBits;
        ++mBits;
        ++rBits;
    }
    delete scaledImage;
    return resultImage;
}

QImage* Algebra::getMultiplyImage(QImage* baseImage)
{
    int size = baseImage->width() * baseImage->height();
    const uchar* bBits = baseImage->constBits();
    const uchar* mBits = scaledImage->constBits();
    uchar* rBits = resultImage->bits();

    for (int i = 0; i < size; ++i) {
        for (int rgb = 0; rgb < 3; ++rgb) {
            int value = (int)*bBits * *mBits;
            if (value > 255) {
                value = 255;
            }
            *rBits = value;

            ++bBits;
            ++mBits;
            ++rBits;
        }
        ++bBits;
        ++mBits;
        ++rBits;
    }
    delete scaledImage;
    return resultImage;
}

QImage* Algebra::getDivideImage(QImage* baseImage)
{
    int size = baseImage->width() * baseImage->height();
    const uchar* bBits = baseImage->constBits();
    const uchar* mBits = scaledImage->constBits();
    uchar* rBits = resultImage->bits();

    for (int i = 0; i < size; ++i) {
        for (int rgb = 0; rgb < 3; ++rgb) {
            *rBits = (*mBits == 0) ? 255 : (int)*bBits / *mBits;

            ++bBits;
            ++mBits;
            ++rBits;
        }
        ++bBits;
        ++mBits;
        ++rBits;
    }
    delete scaledImage;
    return resultImage;
}
