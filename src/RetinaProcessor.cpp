#include "RetinaProcessor.h"

RetinaProcessor::RetinaProcessor(QImage* originImage, QImage* maskImage) :
    ImageProcessor(originImage),
    imageSize(originImage->width() * originImage->height())
{
    maskPtr = new bool[imageSize];
    const uchar* bits = maskImage->constBits();
    maskSize = 0;
    for (int i = 0; i < imageSize; ++i) {
        if (*bits == 0) {
            maskPtr[i] = false;
        } else {
            maskPtr[i] = true;
            ++maskSize;
        }
        bits += PIXEL_SIZE;
    }
}

RetinaProcessor::~RetinaProcessor()
{
    delete []maskPtr;
}

QImage* RetinaProcessor::getGrayScaleImage()
{
    if (grayScaleImage) {
        return grayScaleImage;
    }
    grayScaleImage = new QImage(originImage->size(), originImage->format());

    // pointer to originBits being processed
    const uchar* originPtr = originImage->constBits();
    // pointer to grayBits being processed
    uchar* grayPtr = grayScaleImage->bits();

    for (int i = 0; i < imageSize; ++i) {
        if (maskPtr[i]) {
            // calculate gray value according to strategy
            uchar grayValue = getGrayValue(originPtr, grayScalePolicy);
            for (int rgb = 0; rgb < 3; ++rgb) {
                *(grayPtr + rgb) = grayValue;
            }
        } else {
            for (int rgb = 0; rgb < 3; ++rgb) {
                *(grayPtr + rgb) = 0;
            }
        }
        // set alpha value to be 255
        *(grayPtr + 3) = MAX_OF_8BITS;
        // move pointer of origin and gray scale pointer to next pixel
        originPtr += PIXEL_SIZE;
        grayPtr += PIXEL_SIZE;
    }
    return grayScaleImage;
}

int* RetinaProcessor::getHistogram()
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
        const uchar* grayPtr = grayScaleImage->constBits();

        int size = grayScaleImage->size().width() *
                grayScaleImage->size().height();
        for (int i = 0; i < size; ++i) {
            if (maskPtr[i]) {
                histogram[*grayPtr]++;
            }
            // point to next pixel
            grayPtr += PIXEL_SIZE;
        }

        // calculate accumulated histogram and weighted sum
        int sum = 0;
        weightedHisSum = 0;
        for (int i = 0; i < RANGE_OF_8BITS; ++i) {
            sum += histogram[i];
            accHistogram[i] = sum;
            weightedHisSum += i * histogram[i];
        }

        isHisCaled = true;
    }
    return histogram;
}

QImage* RetinaProcessor::getEqualImage()
{
    if (equalImage) {
        return equalImage;
    }
    getHistogram();
    getGrayScaleImage();

    // min level of rgb, within [0, 256)
    int minLevel;
    for (int i = 0; i < RANGE_OF_8BITS; ++i) {
        if (histogram[i] > 0) {
            minLevel = i;
            break;
        }
    }
    // max level of rgb, within [0, 256)
    int maxLevel;
    for (int i = RANGE_OF_8BITS - 1; i >= 0; --i) {
        if (histogram[i] > 0) {
            maxLevel = i;
            break;
        }
    }

    equalImage = new QImage(originImage->width(), originImage->height(),
                            originImage->format());
    uchar* bits = equalImage->bits();
    const uchar* oBits = grayScaleImage->constBits();

    int size = equalImage->width() * equalImage->height();
    for (int i = 0; i < size; ++i) {
        uchar value;
        if (maskPtr[i]) {
            value = (double)accHistogram[*oBits] / maskSize * MAX_OF_8BITS;
        } else {
            value = 0;
        }
        for (int rgb = 0; rgb < 3; ++rgb) {
            *bits = value;
            ++bits;
            ++oBits;
        }
        ++bits;
        ++oBits;
    }
    return equalImage;
}
