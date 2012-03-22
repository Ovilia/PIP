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
    virtual ~LinearFilter();

protected:
    int* kernelPtrInt;
    double* kernelPtrDouble;

    // linear filter use convolution to do filtering
    virtual uchar doFiltering(int x, int y, ColorOffset offset);

    // helper function for doing int filtering
    uchar doFilteringInt(int x, int y, ColorOffset offset);
    // helper function for doing double filtering
    uchar doFilteringDouble(int x, int y, ColorOffset offset);
};

#endif // LINEARFILTER_H
