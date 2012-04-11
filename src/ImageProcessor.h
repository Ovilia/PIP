#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#define TEAM_WORK

#include <QImage>
#include <QString>

#include "ImagePolicy.h"

class ImageProcessor
{
public:
    ImageProcessor(QString fileName);
    ~ImageProcessor();

    QImage* getOriginImage();
    QImage* getGrayScaleImage();
    QImage* getBinaryImage();
    QImage* getEqualImage();

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

#ifdef TEAM_WORK
    // set contrast value, contrast should between [-50, 100]
    QImage* getContrastImage(int contrast);

    // set brightness, brightness should between [-150, 150]
    QImage* getBrightnessImage(int brightness);
#endif

    /**
     * End of actions that can be undo and redo
     */

    static const int RANGE_OF_8BITS = 256;
    static const int MAX_OF_8BITS = RANGE_OF_8BITS - 1;

private:
    QString fileName;
    QImage* originImage;
    QImage* grayScaleImage;
    QImage* binaryImage;
    QImage* equalImage;
    QImage* contrastImage;
    QImage* brightImage;

    static const int PIXEL_SIZE = 4;

    // reset all to uncalculated state
    void resetUncalculated();
    // if histogram is calculated
    bool isHisCaled, isRgbHisCaled;
    int histogram[RANGE_OF_8BITS];
    int rgbHistogram[RANGE_OF_8BITS][3];
    // accumulated histogram
    int accHistogram[RANGE_OF_8BITS];
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
