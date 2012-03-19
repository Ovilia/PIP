#include <QtCore/qmath.h>
#include <QImage>
#include <QPixmap>

#include "ImageProcessor.h"
#include "ImagePolicy.h"

ImageProcessor::ImageProcessor(QString fileName) :
    originImage(0),
    grayScaleImage(0),
    isHisCaled(false),
    isRgbHisCaled(false),
    weightedHisSum(0),
    otsuThreshold(0),
    entropyThreshold(0),
    lowerThreshold(0),
    higherThreshold(0),
    isOtsuCaled(false),
    isEntropyCaled(false),
    grayScalePolicy(ImagePolicy::MATCH_LUMINANCE),
    thresholdPolicy(ImagePolicy::OTSU)
{
    setImage(fileName);
}

ImageProcessor::~ImageProcessor()
{
    delete originImage;
    delete grayScaleImage;
    delete binaryImage;
}

void ImageProcessor::setImage(QString fileName)
{
    grayScaleImage = 0;
    isHisCaled = false;
    isRgbHisCaled = false;
    isOtsuCaled = false;
    isEntropyCaled = false;

    this->fileName = fileName;
    originImage = new QImage(fileName);
    if (originImage->format() != QImage::Format_RGB32) {
        *originImage = originImage->convertToFormat(QImage::Format_RGB32);
    }
}

void ImageProcessor::setGrayScalePolicy(ImagePolicy::GrayScalePolicy policy)
{
    grayScalePolicy = policy;
    grayScaleImage = 0;
    isHisCaled = false;
    isOtsuCaled = false;
    isEntropyCaled = false;
}

ImagePolicy::GrayScalePolicy ImageProcessor::getGrayScalePolicy()
{
    return grayScalePolicy;
}

void ImageProcessor::setThresholdPolicy(
        ImagePolicy::ThresholdPolicy policy, int lower, int higher)
{
    thresholdPolicy = policy;
    if (policy == ImagePolicy::COSTUMED) {
        lowerThreshold = lower;
        higherThreshold = higher;
    }
}

ImagePolicy::ThresholdPolicy ImageProcessor::getThresholdPolicy()
{
    return thresholdPolicy;
}

int ImageProcessor::getLowerThreshold()
{
    switch (thresholdPolicy) {
    case ImagePolicy::COSTUMED:
        return lowerThreshold;
    default:
        return 0;
    }
}

int ImageProcessor::getHigherThreshold()
{
    switch (thresholdPolicy) {
    case ImagePolicy::OTSU:
        return getOtsuThreshold();
    case ImagePolicy::ENTROPY:
        return getEntropyThreshold();
    case ImagePolicy::COSTUMED:
        return higherThreshold;
    }
    return 0;
}

int ImageProcessor::getOtsuThreshold()
{
    // lazy calculation
    if (!isOtsuCaled) {
        // Make sure histogram is calculate
        getHistogram();

        // Calculate when threshold value = 0,
        // then calculate on the base of former outcome

        // Sum of pixels with index below thresholdValue
        int belowSum = 0;
        // Sum of pixels with index upper than threshold value
        int upperSum = accumulatedHistogram[RANGE_OF_8BITS - 1];

        // Sum of pixels multiplies index value below thresholdValue
        int belowWeighted = 0;
        // Sum of pixels multiplies index value below threshold value
        int upperWeighted = weightedHisSum;

        // Average of pixels below thresholdValue
        double belowAverage = 0;
        // Average of pixels upper than thresholdValue
        double upperAverage;
        if (upperSum == 0)
        {
            upperAverage = 0;
        }
        else
        {
            upperAverage = (double)upperWeighted / upperSum;
        }

        // Max variance between two parts
        double maxVarianceBetween = belowSum * upperSum
                * (belowAverage - upperAverage) * (belowAverage - upperAverage);
        otsuThreshold = 0;

        // Loop threshold value to get max variance between two parts
        for (int thresholdValue = 0; thresholdValue < RANGE_OF_8BITS; ++thresholdValue)
        {
            belowSum += histogram[thresholdValue];
            upperSum -= histogram[thresholdValue];

            if (belowSum == 0)
            {
                continue;
            }
            if (upperSum == 0)
            {
                break;
            }

            belowWeighted += histogram[thresholdValue] * thresholdValue;
            upperWeighted -= histogram[thresholdValue] * thresholdValue;

            belowAverage = (double)belowWeighted / belowSum;
            upperAverage = (double)upperWeighted / upperSum;

            // Variance between below and upper part
            double variaceBetween = (double)belowSum * upperSum
                    * (belowAverage - upperAverage) * (belowAverage - upperAverage);

            // Update threshold value if variance between two parts are
            // larger than maxVarianceBetween
            if (variaceBetween > maxVarianceBetween)
            {
                maxVarianceBetween = variaceBetween;
                otsuThreshold = thresholdValue;
            }
        }
        isOtsuCaled = true;
    }
    return otsuThreshold;
}

int ImageProcessor::getEntropyThreshold()
{
    // lazy calculation
    if (!isEntropyCaled) {
        // Make sure histogram is calculate
        getHistogram();

        // Sum of x * log(x) with index below thresholdValue
        double belowSumXLogX = 0;
        // Sum of x * log(x) with index upper than threshold value
        double upperSumXLogX = 0;
        for (int i = 0; i < RANGE_OF_8BITS; ++i)
        {
            if (histogram[i] != 0)
            {
                // Log on base e
                upperSumXLogX += histogram[i] * qLn((double)histogram[i]);
            }
        }

        double maxEntropy = 0;

        for (int thresholdValue = 0; thresholdValue < RANGE_OF_8BITS; ++thresholdValue)
        {
            if (histogram[thresholdValue] == 0)
            {
                continue;
            }

            int upperSum = accumulatedHistogram[RANGE_OF_8BITS - 1]
                    - accumulatedHistogram[thresholdValue];
            if (upperSum == 0)
            {
                break;
            }

            double xLogX = histogram[thresholdValue] * qLn(histogram[thresholdValue]);
            belowSumXLogX += xLogX;
            upperSumXLogX -= xLogX;

            double belowEntropy = qLn(accumulatedHistogram[thresholdValue])
                    - belowSumXLogX / accumulatedHistogram[thresholdValue];
            double upperEntropy = qLn(upperSum)
                    - upperSumXLogX / upperSum;
            double entropy = belowEntropy + upperEntropy;

            if (entropy > maxEntropy)
            {
                entropyThreshold = thresholdValue;
                maxEntropy = entropy;
            }
        }
        isEntropyCaled = true;
    }
    return entropyThreshold;
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

QImage* ImageProcessor::getGrayScaleImage()
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
            uchar grayValue = getGrayValue(originPtr, grayScalePolicy);
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

        // calculate accumulated histogram and weighted sum
        int sum = 0;
        weightedHisSum = 0;
        for (int i = 0; i < RANGE_OF_8BITS; ++i) {
            sum += histogram[i];
            accumulatedHistogram[i] = sum;
            weightedHisSum += i * histogram[i];
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

QImage* ImageProcessor::getBinaryImage()
{
    // make sure histogram is calculated
    getHistogram();

    int lower = getLowerThreshold();
    int higher = getHigherThreshold();

    int width = grayScaleImage->size().width();
    int height = grayScaleImage->size().height();
    binaryImage = new QImage(width, height, originImage->format());

    const uchar* grayPtr = grayScaleImage->bits();
    uchar* binaryPtr = binaryImage->bits();

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            // the middle part is black, the rest is white
            if (*grayPtr <= lower || *grayPtr > higher) {
                *binaryPtr = 255;
                *(binaryPtr + 1) = 255;
                *(binaryPtr + 2) = 255;
            }
            grayPtr += 4;
            binaryPtr += 4;
        }
    }
    return binaryImage;
}
