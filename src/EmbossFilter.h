#ifndef EMBOSSFILTER_H
#define EMBOSSFILTER_H

#include "LinearFilter.h"

class EmbossFilter : public LinearFilter
{
public:
    EmbossFilter(QImage* image, const int kernelRadio, const bool isColored,
                 ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    ~EmbossFilter();

private:
    double* kernelPtr;
    int kernelLength;
};

#endif // EMBOSSFILTER_H
