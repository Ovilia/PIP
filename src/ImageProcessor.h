#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QString>

#include "ImageStrategy.h"

class ImageProcessor
{
public:
    ImageProcessor(QString fileName);
    ~ImageProcessor();

    QImage* getOriginImage();
    QImage* getGrayScaleImage(ImageStrategy::GrayScaleStrategy strategy);
    int* getHistogram();
    int* getRgbHistogram();

    void setImage(QString fileName);

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

    inline uchar getGrayValue(
            uchar* rgb, ImageStrategy::GrayScaleStrategy strategy);
};

#endif // IMAGEPROCESSOR_H
