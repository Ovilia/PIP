#include <QImage>

#include "MeanFilter.h"

MeanFilter::MeanFilter(QImage* image, const int kernelRadio,
                       const bool isColored,
                       ImagePolicy::BorderPolicy policy) :
    LinearFilter(image, kernelRadio, isColored, policy),
    kernelLength((2 * kernelRadio + 1) * (2 * kernelRadio + 1))
{
    kernelPtr = new double[kernelLength];
    for (int i = 0; i < kernelLength; ++i) {
        kernelPtr[i] = 1.0 / kernelLength;
    }
    changeKernel(kernelPtr);
}

MeanFilter::~MeanFilter()
{
    delete []kernelPtr;
}
