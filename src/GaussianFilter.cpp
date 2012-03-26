#include <cmath>

#include "GaussianFilter.h"
#include "ImagePolicy.h"

GaussianFilter::GaussianFilter(QImage* image, const double sigma,
                               const bool isColored,
                               ImagePolicy::BorderPolicy policy) :
    // if radio is less than 1, set it to be 1
    LinearFilter(image, (int)(sigma * RADIO_DIVIDED_BY_SIGMA) < 1 ?
                     1 : (int)(sigma * RADIO_DIVIDED_BY_SIGMA),
                 isColored, policy),
    kernelLength((2 * kernelRadio + 1) * (2 * kernelRadio + 1)),
    sigma(sigma)
{
    kernelPtr = new double[kernelLength];

    // init kernel
    double coefficient = 0.5 / M_PI / sigma / sigma;
    double sum = 0;
    int index = 0;
    for (int i = -kernelRadio; i <= kernelRadio; ++i) {
        for (int j = -kernelRadio; j <= kernelRadio; ++j) {
            double kernel = coefficient *
                    exp((i * i + j * j) / (-2) / sigma / sigma);
            kernelPtr[index] = kernel;
            ++index;
            sum += kernel;
        }
    }
    // make sure sum is 1
    for (int i = 0; i < kernelLength; ++i) {
        kernelPtr[i] /= sum;
    }

    LinearFilter::changeKernel(kernelPtr);
}

GaussianFilter::~GaussianFilter()
{
    delete kernelPtr;
}

double GaussianFilter::getSigma()
{
    return sigma;
}
