#include <QtDebug>
#include <QImage>
#include <QPixmap>

#include "ImageProcessor.h"
#include "ImageStrategy.h"

ImageProcessor::ImageProcessor(QString fileName)
{
    this->fileName = fileName;
    originImage = new QImage(fileName);
    grayScaleImage = 0;
}

ImageProcessor::~ImageProcessor()
{
    delete originImage;
    delete grayScaleImage;
}

QImage* ImageProcessor::getOriginImage()
{
    return originImage;
}

inline uchar ImageProcessor::getGrayValue(
        uchar* rgb, ImageStrategy::GrayScaleStrategy strategy)
{
    switch(strategy) {
    case ImageStrategy::GREEN_ONLY:
        return *(rgb + 1);
    case ImageStrategy::MATCH_LUMINANCE:
        return (*rgb) * 0.3 + *(rgb + 1) * 0.59 + *(rgb + 2) * 0.11;
    case ImageStrategy::RGB_AVERAGE:
        return (*rgb + *(rgb + 1) + *(rgb + 2)) / 3;
    default:
        return 0;
    }
}

QImage* ImageProcessor::getGrayScaleImage(
        ImageStrategy::GrayScaleStrategy strategy)
{
    // lazy calculation
    if (!grayScaleImage) {
        int width = originImage->size().width();
        int height = originImage->size().height();
        int size = width * height;

        // gray scale image of origin size
        grayScaleImage = new QImage(width, height, originImage->format());

        // pointer to originBits being processed
        uchar* originPtr = originImage->bits();
        // pointer to grayBits being processed
        uchar* grayPtr = grayScaleImage->bits();

        for (int i = 0; i < size; ++i) {
            // calculate gray value according to strategy
            uchar grayValue = getGrayValue(originPtr, strategy);
            // set rgb value to be grayValue
            for (int rgb = 0; rgb < 3; ++rgb) {
                *(grayPtr + rgb) = grayValue;
            }
            // set alpha value to be 255
            *(grayPtr + 3) = MAX_OF_8BITS;
            // move pointer of origin and gray scale pointer to next pixel
            originPtr += 4;
            grayPtr += 4;
        }
    }
    return grayScaleImage;
}
