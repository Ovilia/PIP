#ifndef MOTIONFILTER_H
#define MOTIONFILTER_H

#include <QImage>

#include "ImagePolicy.h"
#include "LinearFilter.h"

class MotionFilter : public LinearFilter
{
public:
    MotionFilter(QImage* image, const int kernelRadio, const bool isColored,
                 ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    ~MotionFilter();

private:
    double* kernelPtr;
    int kernelLength;
};

#endif // MOTIONFILTER_H
