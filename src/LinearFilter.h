#ifndef LINEARFILTER_H
#define LINEARFILTER_H

#include <QImage>

#include "Filter.h"

class LinearFilter : public Filter
{
public:
    LinearFilter(QImage* image, int kernelRadio, int* kernelPtr,
                 ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    LinearFilter(QImage* image, int kernelRadio, double* kernelPtr,
                 ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    ~LinearFilter();

    virtual QImage* getFilteredImage();

protected:
    int* kernelPtrInt;
    double* kernelPtrDouble;

    // filtering algorithm defined by concrete filters
    virtual uchar doFiltering(int x, int y, ColorOffset offset) = 0;
};

#endif // LINEARFILTER_H
