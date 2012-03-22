#ifndef LINEARFILTER_H
#define LINEARFILTER_H

#include <QImage>

#include "Filter.h"
#include "Operator.h"

class LinearFilter : public Filter
{
public:
    LinearFilter(QImage* image, const int kernelRadio,
                 ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    LinearFilter(QImage* image, const int kernelRadio, const int* kernelPtr,
                 ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    LinearFilter(QImage* image, const int kernelRadio, const double* kernelPtr,
                 ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    virtual ~LinearFilter();

    void changeKernel(const int* kernel);
    void changeKernel(const double* kernel);

    friend class Operator;

protected:
    const int* kernelPtrInt;
    const double* kernelPtrDouble;

    // linear filter use convolution to do filtering
    virtual uchar doFiltering(int x, int y, ColorOffset offset);

    // helper function for doing int filtering
    uchar doFilteringInt(int x, int y, ColorOffset offset);
    // helper function for doing double filtering
    uchar doFilteringDouble(int x, int y, ColorOffset offset);
};

#endif // LINEARFILTER_H
