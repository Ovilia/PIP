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
    int max;
    int maxIndex = -1;
    for (int i = 0; i < nMax; ++i) {
        max = -1;
        for (int j = 0; j < kernelLength; ++j) {
            if (value[j] > max) {
                max = value[j];
                maxIndex = j;
            }
        }
        value[maxIndex] = -1;
    }
    delete []value;
    return max;
}
