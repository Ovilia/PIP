#include "PointFilter.h"

#include <qmath.h>

PointFilter::PointFilter(QImage *image) :
    originImage(image),
    comicImage(0),
    sepiaImage(0)
{
}

PointFilter::~PointFilter()
{
    if (comicImage) {
        delete comicImage;
    }
}

QImage* PointFilter::getComicImage()
{
    if (comicImage) {
        return comicImage;
    }

    comicImage = new QImage(originImage->size(), originImage->format());

    const uchar* oBits = originImage->constBits();
    uchar* bits = comicImage->bits();
    int size = originImage->width() * originImage->height();
    for (int i = 0; i < size; ++i) {
        int blue = (int)*(oBits++);
        int green = (int)*(oBits++);
        int red = (int)*(oBits++);

        int tmp = qAbs(blue - green + blue + red);
        int newBlue = qAbs(green - blue + green + red) * red / 256;
        int newGreen = tmp * red / 256;
        int newRed = tmp * green / 256;
        uchar gray = getPixel((newBlue + newGreen + newRed) / 3);
        for (int rgb = 0; rgb < 3; ++rgb) {
            *bits = gray;
            ++bits;
        }

        ++oBits;
        ++bits;
    }
    return comicImage;
}

QImage* PointFilter::getSepiaImage()
{
    if (sepiaImage) {
        return sepiaImage;
    }

    sepiaImage = new QImage(originImage->size(), originImage->format());

    const uchar* oBits = originImage->constBits();
    uchar* bits = sepiaImage->bits();
    int size = originImage->width() * originImage->height();
    for (int i = 0; i < size; ++i) {
        int blue = (int)*(oBits++);
        int green = (int)*(oBits++);
        int red = (int)*(oBits++);

        *bits = getPixel(0.272 * red + 0.534 * green + 0.131 * blue);
        ++bits;
        *bits = getPixel(0.349 * red + 0.686 * green + 0.168 * blue);
        ++bits;
        *bits = getPixel(0.393 * red + 0.769 * green + 0.189 * blue);
        ++bits;

        ++oBits;
        ++bits;
    }
    return sepiaImage;
}

inline uchar PointFilter::getPixel(double value)
{
    if (value > 255) {
        return 255;
    } else if (value < 0) {
        return 0;
    } else {
        return static_cast<unsigned char>(value);
    }
}
