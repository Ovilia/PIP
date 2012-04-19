#include "EmbossFilter.h"

EmbossFilter::EmbossFilter(QImage* image, const int kernelRadio,
                           const bool isColored,
                           ImagePolicy::BorderPolicy policy) :
    LinearFilter(image, kernelRadio, isColored, policy),
    kernelLength((2 * kernelRadio + 1) * (2 * kernelRadio + 1))
{
    kernelPtr = new double[kernelLength];
    int length = 2 * kernelRadio + 1;
    int index = 0;
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < length; ++j) {
            if (i + j <= kernelRadio) {
                // top-left part
                kernelPtr[index] = -1;
            } else if (i + j >= 3 * kernelRadio) {
                // bottom-right part
                kernelPtr[index] = 1;
            } else {
                // center part
                kernelPtr[index] = 0;
            }
            ++index;
        }
    }
    kernelPtr[kernelLength / 2] = 1;
    changeKernel(kernelPtr);
}

EmbossFilter::~EmbossFilter()
{
    delete []kernelPtr;
}
