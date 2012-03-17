#include <QtDebug>
#include <QImage>
#include <QPixmap>

#include "ImageProcessor.h"
#include "ImagePolicy.h"

ImageProcessor::ImageProcessor(QString fileName)
{
    setImage(fileName);
}

ImageProcessor::~ImageProcessor()
{
    delete originImage;
    delete grayScaleImage;
}

void ImageProcessor::setImage(QString fileName)
{
    this->fileName = fileName;
    grayScaleImage = 0;
    isHisCaled = false;
    isRgbHisCaled = false;

    originImage = new QImage(fileName);
    if (originImage->format() != QImage::Format_RGB32) {
        *originImage = originImage->convertToFormat(QImage::Format_RGB32);
    }
}

QImage* ImageProcessor::getOriginImage()
{
    return originImage;
}

inline uchar ImageProcessor::getGrayValue(
        uchar* rgb, ImagePolicy::GrayScalePolicy policy)
{
    switch(policy) {
    case ImagePolicy::GREEN_ONLY:
        return *(rgb + 1);
    case ImagePolicy::MATCH_LUMINANCE:
        return ((*rgb) * 28 + *(rgb + 1) * 151 + *(rgb + 2) * 77) >> 8;
    case ImagePolicy::RGB_AVERAGE:
        return (*rgb + *(rgb + 1) + *(rgb + 2)) / 3;
    default:
        return 0;
    }
}

QImage* ImageProcessor::getGrayScaleImage(
        ImagePolicy::GrayScalePolicy policy
        = ImagePolicy::MATCH_LUMINANCE)
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
            uchar grayValue = getGrayValue(originPtr, policy);
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

int* ImageProcessor::getHistogram()
{
    // lazy calculation
    if (!isHisCaled) {
        // make sure gray scale image is calculated
        getGrayScaleImage();
        // init arrays
        for (int i = 0; i < RANGE_OF_8BITS; ++i) {
            histogram[i] = 0;
        }

        // pointer to pixels being processed which will not change
        // pixels in grayImage
        const uchar* grayPtr = grayScaleImage->bits();

        int size = grayScaleImage->size().width() *
                grayScaleImage->size().height();
        for (int i = 0; i < size; ++i) {
            histogram[*grayPtr]++;
            // point to next pixel
            grayPtr += 4;
        }

        isHisCaled = true;
    }
    return histogram;
}

int* ImageProcessor::getRgbHistogram()
{
    // lazy calculation
    if (!isRgbHisCaled) {
        // init arrays
        for (int i = 0; i < RANGE_OF_8BITS; ++i) {
            for (int j = 0; j < 3; ++j) {
                rgbHistogram[i][j] = 0;
            }
        }

        // pointer to pixels being processed which will not change
        // pixels in originImage
        const uchar* originPtr = originImage->bits();

        int size = originImage->size().width() *
                originImage->size().height();
        for (int i = 0; i < size; ++i) {
            // blue
            rgbHistogram[*originPtr][2]++;
            // green
            rgbHistogram[*(originPtr + 1)][1]++;
            // red
            rgbHistogram[*(originPtr + 2)][0]++;
            // point to next pixel
            originPtr += 4;
        }
    }
    return *rgbHistogram;
}
