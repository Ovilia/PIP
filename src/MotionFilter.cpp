#include "MotionFilter.h"

MotionFilter::MotionFilter(QImage* image, const int kernelRadio,
                           const bool isColored,
                           ImagePolicy::BorderPolicy policy) :
    LinearFilter(image, kernelRadio, isColored, policy),
    kernelLength((2 * kernelRadio + 1) * (2 * kernelRadio + 1))
{
    kernelPtr = new double[kernelLength];
    int length = 2 * kernelRadio + 1;
    for (int i = 0; i < length; ++i) {
        kernelPtr[i * length + i] = 1.0 / length;
    }
    changeKernel(kernelPtr);
}

MotionFilter::~MotionFilter()
{
    delete []kernelPtr;
}
