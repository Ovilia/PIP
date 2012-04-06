#include <QtCore/qmath.h>
#include <QImage>
#include <QPixmap>

#include "ImageProcessor.h"
#include "ImagePolicy.h"

ImageProcessor::ImageProcessor(QString fileName) :
    originImage(0),
    currentImage(0),
    grayScaleImage(0),
    binaryImage(0),
    usedBuffer(0),
    undoBuffer(0),
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
    for (int i = 0; i < HISTORY_COUNT; ++i) {
        bufferedImage[i] = 0;
    }
    setImage(fileName);
    originImage = currentImage;
}

ImageProcessor::~ImageProcessor()
{
    if (originImage) {
        delete originImage;
    }
    for (int i = 0; i < HISTORY_COUNT; ++i) {
        if (!bufferedImage[i]) {
            delete bufferedImage[i];
        }
    }
    if (grayScaleImage) {
        delete grayScaleImage;
    }
    if (binaryImage) {
        delete binaryImage;
    }
}

void ImageProcessor::setImage(QString fileName)
{
    this->fileName = fileName;
    bufferCurrentImage();
    currentImage = new QImage(fileName);
    doFormatProcess(currentImage);
}

void ImageProcessor::doFormatProcess(QImage *image)
{
    if (image == 0) {
        return;
    }
    if (image->format() != QImage::Format_RGB32) {
        if (image->format() == QImage::Format_ARGB32) {
            // set pixel with alpha 0 to be white
            uchar* bits = image->bits();
            int size = image->width() * image->height();
            for (int i = 0; i < size; ++i) {
                int alpha = *(bits + 3);
                if (alpha == 0) {
                    *bits = MAX_OF_8BITS;
                    *(bits + 1) = MAX_OF_8BITS;
                    *(bits + 2) = MAX_OF_8BITS;
                } else if (alpha != MAX_OF_8BITS) {
                    // if alpha is not 255, set rgb to be semitransparent
                    *bits = *bits * alpha / MAX_OF_8BITS +
                            MAX_OF_8BITS - alpha;
                    *(bits + 1) = *(bits + 1) * alpha / MAX_OF_8BITS +
                            MAX_OF_8BITS - alpha;
                    *(bits + 2) = *(bits + 2) * alpha / MAX_OF_8BITS +
                            MAX_OF_8BITS - alpha;
                }
                bits += 4;
            }
        }
        *image = image->convertToFormat(QImage::Format_RGB32);
    }
}

void ImageProcessor::doContrast(int contrast)
{
    // currentImage will be changed, buffer it so that undo is possible
    bufferCurrentImage();

    // make sure histogram is calculated
    getHistogram();

    const uchar* originPtr = currentImage->constBits();
    int height = currentImage->height();
    int width = currentImage->width();

    QImage* newImage = new QImage(width, height, currentImage->format());
    uchar* newPtr = newImage->bits();

    int averageBrightness;
    int originColor = 0;
    int adjustedColor = 0;

    // compute averageBrightness
    long brightnessSum = 0;
    for (int i = 0; i < RANGE_OF_8BITS; ++i) {
        brightnessSum += histogram[i];
    }
    averageBrightness = brightnessSum / width / height;

    // adjust each pixel
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            for (int rgb = 0; rgb < 3; ++rgb) {
                originColor = *(originPtr +
                                PIXEL_SIZE * (width * j + i) + rgb);
                adjustedColor = (int)((double)averageBrightness + (double)
                                      (originColor - averageBrightness)
                                      * (double)contrast / 100);
                if (adjustedColor < 0) {
                    adjustedColor = 0;
                }
                if (adjustedColor > MAX_OF_8BITS) {
                    adjustedColor = MAX_OF_8BITS;
                }
                *(newPtr + PIXEL_SIZE * (width * j + i) + rgb) =
                        adjustedColor;
            }
        }
    }
    currentImage = newImage;
}

void ImageProcessor::doBrightness(int brightness)
{
    // currentImage will be changed, buffer it so that undo is possible
    bufferCurrentImage();

    // TODO: do brightness now
    // post-condition: result is in currentImage
}

void ImageProcessor::doScale(int newWidth, int newHeight,
                             ImagePolicy::ScalePolicy policy)
{
    bufferCurrentImage();

    QImage* newImage = new QImage(newWidth, newHeight, currentImage->format());
    switch (policy) {
    case ImagePolicy::SP_NEAREST:
        doNearestScale(currentImage, newImage);
        break;

    case ImagePolicy::SP_BILINEAR:
        break;
    }

    currentImage = newImage;
}

void ImageProcessor::doNearestScale(QImage* oldImage, QImage* newImage)
{
    const uchar* originPtr = oldImage->constBits();
    int height = currentImage->height();
    int width = currentImage->width();
    uchar* newPtr = newImage->bits();

    // TODO: do nearest scaling here
}

void ImageProcessor::bufferCurrentImage()
{
    if (!currentImage) {
        return;
    }
    if (usedBuffer >= HISTORY_COUNT) {
        // buffer is full, remove the oldest one
        delete bufferedImage[HISTORY_COUNT - 1];
        // move other buffer to be older
        for (int i = 0; i < HISTORY_COUNT - 1; ++i) {
            bufferedImage[i] = bufferedImage[i + 1];
        }
        usedBuffer = HISTORY_COUNT - 1;
    }
    // copy to last position used if is not full
    bufferedImage[usedBuffer] = currentImage;
    ++usedBuffer;
    resetUncalculated();
}

void ImageProcessor::resetUncalculated()
{
    // reset to uncalculated state
    grayScaleImage = 0;
    binaryImage = 0;
    isHisCaled = false;
    isRgbHisCaled = false;
    isOtsuCaled = false;
    isEntropyCaled = false;
}

bool ImageProcessor::undo()
{
    if (undoBuffer < usedBuffer) {
        // undo last action
        currentImage = bufferedImage[undoBuffer];
        ++undoBuffer;
    }
    resetUncalculated();
    return isUndoable();
}

bool ImageProcessor::redo()
{
    if (undoBuffer > 0) {
        // redo last action
        --undoBuffer;
        currentImage = bufferedImage[undoBuffer];
    }
    resetUncalculated();
    return isRedoable();
}

inline bool ImageProcessor::isUndoable() const
{
    return undoBuffer < usedBuffer;
}

inline bool ImageProcessor::isRedoable() const
{
    return undoBuffer > 0;
}

void ImageProcessor::setGrayScalePolicy(ImagePolicy::GrayScalePolicy policy)
{
    if (grayScalePolicy != policy) {
        grayScalePolicy = policy;
        if (grayScaleImage) {
            delete grayScaleImage;
            grayScaleImage = 0;
        }
        if (binaryImage) {
            delete binaryImage;
            binaryImage = 0;
        }
        isHisCaled = false;
        isOtsuCaled = false;
        isEntropyCaled = false;
    }
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

QImage* ImageProcessor::getCurrentImage()
{
    return currentImage;
}

inline uchar ImageProcessor::getGrayValue(
        const uchar* rgb, ImagePolicy::GrayScalePolicy policy)
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
        int width = currentImage->size().width();
        int height = currentImage->size().height();
        int size = width * height;

        // gray scale image of origin size
        if (grayScaleImage) {
            delete grayScaleImage;
        }
        grayScaleImage = new QImage(width, height, currentImage->format());

        // pointer to originBits being processed
        const uchar* originPtr = currentImage->constBits();
        // pointer to grayBits being processed
        uchar* grayPtr = grayScaleImage->bits();

        for (int i = 0; i < size; ++i) {
            // calculate gray value according to strategy
            uchar grayValue = getGrayValue(originPtr, grayScalePolicy);
            for (int rgb = 0; rgb < 3; ++rgb) {
                *(grayPtr + rgb) = grayValue;
            }
            // set alpha value to be 255
            *(grayPtr + 3) = MAX_OF_8BITS;
            // move pointer of origin and gray scale pointer to next pixel
            originPtr += PIXEL_SIZE;
            grayPtr += PIXEL_SIZE;
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
        const uchar* grayPtr = grayScaleImage->constBits();

        int size = grayScaleImage->size().width() *
                grayScaleImage->size().height();
        for (int i = 0; i < size; ++i) {
            histogram[*grayPtr]++;
            // point to next pixel
            grayPtr += PIXEL_SIZE;
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
        // pixels in currentImage
        const uchar* originPtr = currentImage->constBits();

        int size = currentImage->width() * currentImage->height();
        for (int i = 0; i < size; ++i) {
            // blue
            rgbHistogram[*originPtr][2]++;
            // green
            rgbHistogram[*(originPtr + 1)][1]++;
            // red
            rgbHistogram[*(originPtr + 2)][0]++;
            // point to next pixel
            originPtr += PIXEL_SIZE;
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

    int width = grayScaleImage->width();
    int height = grayScaleImage->height();
    int size = width * height;
    if (binaryImage) {
        delete binaryImage;
    }
    binaryImage = new QImage(width, height, currentImage->format());

    const uchar* grayPtr = grayScaleImage->constBits();
    uchar* binaryPtr = binaryImage->bits();

    for (int i = 0; i < size; ++i) {
        if (*grayPtr <= lower || *grayPtr > higher) {
            // set two sides of threshold white
            // blue
            *binaryPtr = MAX_OF_8BITS;
            // green
            *(binaryPtr + 1) = MAX_OF_8BITS;
            // red
            *(binaryPtr + 2) = MAX_OF_8BITS;
            // alpha
            *(binaryPtr + 3) = MAX_OF_8BITS;
        } else {
            // blue
            *binaryPtr = 0;
            // green
            *(binaryPtr + 1) = 0;
            // red
            *(binaryPtr + 2) = 0;
            // alpha
            *(binaryPtr + 3) = MAX_OF_8BITS;
        }
        grayPtr += PIXEL_SIZE;
        binaryPtr += PIXEL_SIZE;
    }
    return binaryImage;
}
