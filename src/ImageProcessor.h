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
    QImage* getGrayScaleImage();
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

    static const int RANGE_OF_8BITS = 256;
    static const int MAX_OF_8BITS = RANGE_OF_8BITS - 1;

private:
    QString fileName;
    QImage* originImage;
    QImage* grayScaleImage;

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
            uchar* rgb, ImagePolicy::GrayScalePolicy policy);
};

#endif // IMAGEPROCESSOR_H
