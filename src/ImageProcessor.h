#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QString>

#include "ImagePolicy.h"

class ImageProcessor
{
public:
    ImageProcessor(QString fileName);
    ~ImageProcessor();

    QImage* getOriginImage();
    QImage* getCurrentImage();
    QImage* getGrayScaleImage();
    QImage* getBinaryImage();
    int* getHistogram();
    int* getRgbHistogram();

    void setImage(QString fileName);

    void setGrayScalePolicy(ImagePolicy::GrayScalePolicy policy);
    ImagePolicy::GrayScalePolicy getGrayScalePolicy();

    // customedValue used only if policy is CUSTOMED
    void setThresholdPolicy(
            ImagePolicy::ThresholdPolicy policy, int lower = 0, int higher = 0);
    ImagePolicy::ThresholdPolicy getThresholdPolicy();

    // lower threshold is always 0 when thresholdPolicy is not CUSTOMED
    int getLowerThreshold();
    int getHigherThreshold();

    // make image to be format RGB32
    void doFormatProcess(QImage* image);

    // undo last action if is legal and return if can
    // still undo after this action
    bool undo();
    // redo last undo action if is legal and return if can
    // still redo after this action
    bool redo();

    /**
     * Start of actions that can be undo and redo,
     * these actions will always call bufferCurrentImage()
     * to buffer current image before any operation
     */

    // set contrast value, contrast should between [-50, 100]
    void doContrast(int contrast);

    // set brightness, brightness should between [-150, 150]
    void doBrightness(int brightness);

    /**
     * End of actions that can be undo and redo
     */

    static const int RANGE_OF_8BITS = 256;
    static const int MAX_OF_8BITS = RANGE_OF_8BITS - 1;

private:
    QString fileName;
    QImage* originImage;
    QImage* currentImage;
    QImage* grayScaleImage;
    QImage* binaryImage;

    static const int PIXEL_SIZE = 4;

    // count of history image that can be undo
    static const int HISTORY_COUNT = 10;
    // amount of buffered image
    int usedBuffer;
    // amount of undid image
    int undoBuffer;
    QImage* bufferedImage[HISTORY_COUNT];
    // set originImage to be bufferedImage
    void bufferCurrentImage();

    // reset all to uncalculated state
    void resetUncalculated();
    // if histogram is calculated
    bool isHisCaled, isRgbHisCaled;
    int histogram[RANGE_OF_8BITS];
    int rgbHistogram[RANGE_OF_8BITS][3];
    // accumulated histogram
    int accumulatedHistogram[RANGE_OF_8BITS];
    // x * histogram[x]
    int weightedHisSum;

    // threshold value calculated using Otsu Algorithm
    int otsuThreshold;
    // threshold value calculated using Entropy Algorithm
    int entropyThreshold;
    // customed threshold value
    int lowerThreshold, higherThreshold;
    bool isOtsuCaled, isEntropyCaled;
    int getOtsuThreshold();
    int getEntropyThreshold();

    // gray scale policy
    ImagePolicy::GrayScalePolicy grayScalePolicy;
    // threshold policy
    ImagePolicy::ThresholdPolicy thresholdPolicy;

    inline uchar getGrayValue(
            const uchar* rgb, ImagePolicy::GrayScalePolicy policy);
};

#endif // IMAGEPROCESSOR_H
