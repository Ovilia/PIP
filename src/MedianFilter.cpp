#include <algorithm>

#include <QImage>

#include "ImagePolicy.h"
#include "MedianFilter.h"

MedianFilter::MedianFilter(QImage* image, const int kernelRadio,
                           const bool isColored,
                           ImagePolicy::BorderPolicy policy) :
    Filter(image, kernelRadio, isColored, policy),
    kernelLength((2 * kernelRadio + 1) * (2 * kernelRadio + 1)),
    nMax(kernelLength / 2)
{
}

MedianFilter::~MedianFilter()
{
}

int MedianFilter::doFiltering(int x, int y, ColorOffset offset)
{
    int* value = new int[kernelLength];
    int index = 0;
    for (int i = -kernelRadio; i <= kernelRadio; ++i) {
        for (int j = -kernelRadio; j <= kernelRadio; ++j) {
            value[index] = getBorderedValue(x + i, y + j, offset);
            ++index;
        }
    }
    std::sort(value, value + kernelLength);
    int result = value[nMax];
    delete []value;
    return result;
}
