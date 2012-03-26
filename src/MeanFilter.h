#ifndef MEANFILTER_H
#define MEANFILTER_H

#include <QImage>

#include "ImagePolicy.h"
#include "LinearFilter.h"

class MeanFilter : public LinearFilter
{
public:
    MeanFilter(QImage* image, const int kernelRadio, const bool isColored,
               ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    ~MeanFilter();

private:
    double* kernelPtr;
    int kernelLength;
};

#endif // MEANFILTER_H
