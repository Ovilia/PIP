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

    // gray scale policy
    ImagePolicy::GrayScalePolicy grayScalePolicy;

    // init members, call when reset image
    void initialize();

    inline uchar getGrayValue(
            uchar* rgb, ImagePolicy::GrayScalePolicy policy);
};

#endif // IMAGEPROCESSOR_H
