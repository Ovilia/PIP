#include <QtCore/qmath.h>

#include "ImagePolicy.h"
#include "Operator.h"

Operator::Operator(QImage* image, const int kernelRadio,
                   const int* xKernel, const int* yKernel,
                   const bool isColored, ImagePolicy::BorderPolicy policy) :
    Filter(image, kernelRadio, isColored, policy),
    kernelRadio(kernelRadio),
    xKernel(xKernel),
    yKernel(yKernel),
    isXEnabled(true),
    isYEnabled(true),
    xOperatedImage(0),
    yOperatedImage(0),
    xyOperatedImage(0)
{
    xFilter = new LinearFilter(image, kernelRadio, xKernel, isColored, policy);
    yFilter = new LinearFilter(image, kernelRadio, yKernel, isColored, policy);
}

Operator::~Operator()
{
    if (xFilter) {
        delete xFilter;
    }
    if (yFilter) {
        delete yFilter;
    }
    if (xOperatedImage) {
        delete xOperatedImage;
    }
    if (yOperatedImage) {
        delete yOperatedImage;
    }
    if (xyOperatedImage) {
        delete xyOperatedImage;
    }
}

QImage* Operator::getFilteredImage()
{
    if (isXEnabled) {
        if (isYEnabled) {
            // xy-filtered image from base class
            return Filter::getFilteredImage();
        } else {
            if (!xOperatedImage) {
                xOperatedImage = xFilter->getFilteredImage();
            }
            return xOperatedImage;
        }
    } else if (isYEnabled) {
        if (!yOperatedImage) {
            yOperatedImage = yFilter->getFilteredImage();
        }
        return yOperatedImage;
    }
    return 0;
}

int Operator::doFiltering(int x, int y, ColorOffset offset)
{
    int xValue = xFilter->doFiltering(x, y, offset);
    int yValue = yFilter->doFiltering(x, y, offset);
    // geometric mean of xValue an yValue
    return (int)qSqrt(xValue * xValue + yValue * yValue);
}

void Operator::setXEnabled(bool isEnabled)
{
    isXEnabled = isEnabled;
}

void Operator::setYEnabled(bool isEnabled)
{
    isYEnabled = isEnabled;
}

bool Operator::getXEnabled()
{
    return isXEnabled;
}

bool Operator::getYEnabled()
{
    return isYEnabled;
}
