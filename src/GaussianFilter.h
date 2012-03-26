#ifndef GaussianFILTER_H
#define GaussianFILTER_H

#include <QImage>

#include "ImagePolicy.h"
#include "LinearFilter.h"

class GaussianFilter : public LinearFilter
{
public:
    GaussianFilter(QImage* image, const double sigma, const bool isColored,
                   ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    ~GaussianFilter();

    double getSigma();

    // radio is set to be 3 times of sigma according to wiki
    static const int RADIO_DIVIDED_BY_SIGMA = 3;

private:
    double* kernelPtr;
    int kernelLength;
    double sigma;
};

#endif // GaussianFILTER_H
