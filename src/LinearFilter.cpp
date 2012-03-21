#include "LinearFilter.h"

LinearFilter::LinearFilter(QImage* image, int kernelRadio, int* kernelPtr,
                           ImagePolicy::BorderPolicy policy) :
    Filter(image, kernelRadio, policy),
    kernelPtrInt(kernelPtr),
    kernelPtrDouble(0)
{
}

LinearFilter::LinearFilter(QImage *image, int kernelRadio, double *kernelPtr,
                           ImagePolicy::BorderPolicy policy) :
    Filter(image, kernelRadio, policy),
    kernelPtrInt(0),
    kernelPtrDouble(kernelPtr)
{
}

LinearFilter::~LinearFilter()
{
}

QImage* LinearFilter::getFilteredImage()
{
    if (filteredImage == 0) {
        if (Filter::isBorderChanged) {
            /**
             * if previously calculated center image and border policy changed,
             * only recalculate the border part
             */
            Filter::resetBorder();
        } else {
            filteredImage = new QImage();
            // TODO
        }
    }
    return filteredImage;
}
