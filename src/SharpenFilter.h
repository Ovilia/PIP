#ifndef SHARPENFILTER_H
#define SHARPENFILTER_H

#include <QImage>

#include "ImagePolicy.h"
#include "LinearFilter.h"

class SharpenFilter : public LinearFilter
{
public:
    SharpenFilter(QImage* image, const int kernelRadio, const bool isColored,
                  ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    ~SharpenFilter();

private:
    double* kernelPtr;
    int kernelLength;
};

#endif // SHARPENFILTER_H
