#include "SharpenFilter.h"

SharpenFilter::SharpenFilter(QImage* image, const int kernelRadio,
                             const bool isColored,
                             ImagePolicy::BorderPolicy policy) :
    LinearFilter(image, kernelRadio, isColored, policy),
    kernelLength((2 * kernelRadio + 1) * (2 * kernelRadio + 1))
{
    kernelPtr = new double[kernelLength];
    for (int i = 0; i < kernelLength; ++i) {
        kernelPtr[i] = -1.0;
    }
    kernelPtr[kernelLength / 2] = kernelLength;
    changeKernel(kernelPtr);
}

SharpenFilter::~SharpenFilter()
{
    delete []kernelPtr;
}
