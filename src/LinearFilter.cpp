#include "LinearFilter.h"

LinearFilter::LinearFilter(QImage* image, const int kernelRadio, const bool isColored,
                           ImagePolicy::BorderPolicy policy) :
    Filter(image, kernelRadio, isColored, policy),
    kernelPtrInt(0),
    kernelPtrDouble(0)
{

}

LinearFilter::LinearFilter(QImage* image,
                           const int kernelRadio, const int* kernelPtr,
                           const bool isColored,
                           ImagePolicy::BorderPolicy policy) :
    Filter(image, kernelRadio, isColored, policy),
    kernelPtrInt(kernelPtr),
    kernelPtrDouble(0)
{
}

LinearFilter::LinearFilter(QImage *image,
                           const int kernelRadio, const double *kernelPtr,
                           const bool isColored,
                           ImagePolicy::BorderPolicy policy) :
    Filter(image, kernelRadio, isColored, policy),
    kernelPtrInt(0),
    kernelPtrDouble(kernelPtr)
{
}

LinearFilter::~LinearFilter()
{
}

void LinearFilter::changeKernel(const int *kernel)
{
    kernelPtrInt = kernel;
    kernelPtrDouble = 0;
    isKernelChanged = true;
}

void LinearFilter::changeKernel(const double *kernel)
{
    kernelPtrDouble = kernel;
    kernelPtrInt = 0;
    isKernelChanged = true;
}

int LinearFilter::doFiltering(int x, int y, ColorOffset offset)
{
    if (kernelPtrInt) {
        return doFilteringInt(x, y, offset);
    } else if (kernelPtrDouble) {
        return (int)doFilteringDouble(x, y, offset);
    } else {
        return 0;
    }
}

int LinearFilter::doFilteringInt(int x, int y, ColorOffset offset)
{
    // convolution
    int sum = 0;
    for (int i = -kernelRadio; i <= kernelRadio; ++i) {
        for (int j = -kernelRadio; j <= kernelRadio; ++j) {
            sum += getBorderedValue(x + i, y + j, offset) *
                    kernelPtrInt[get2DIndex(2 * kernelRadio + 1,
                                            i + kernelRadio,
                                            j + kernelRadio)];
        }
    }
    return sum;
}

double LinearFilter::doFilteringDouble(int x, int y, ColorOffset offset)
{
    // convolution
    double sum = 0;
    for (int i = -kernelRadio; i <= kernelRadio; ++i) {
        for (int j = -kernelRadio; j <= kernelRadio; ++j) {
            sum += getBorderedValue(x + i, y + j, offset) *
                    kernelPtrDouble[get2DIndex(2 * kernelRadio + 1,
                                               i + kernelRadio,
                                               j + kernelRadio)];
        }
    }
    return sum;
}
