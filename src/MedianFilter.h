#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <QImage>

#include "Filter.h"
#include "ImagePolicy.h"

class MedianFilter : public Filter
{
public:
    MedianFilter(QImage* image, const int kernelRadio, const bool isColored,
                 ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    ~MedianFilter();

    virtual int doFiltering(int x, int y, ColorOffset offset);

private:
    int kernelLength;
    // n'th max number in neighorhood, half size of kernelLength
    int nMax;
};

#endif // MEDIANFILTER_H
