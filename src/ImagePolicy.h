#ifndef IMAGEPOLICY_H
#define IMAGEPOLICY_H

class ImagePolicy
{
public:
    enum GrayScalePolicy {
        MATCH_LUMINANCE = 0,
        RGB_AVERAGE,
        GREEN_ONLY
    };

    enum ThresholdPolicy {
        OTSU = 0,
        ENTROPY,
        COSTUMED
    };

    enum BorderPolicy {
        BLACK = 0,
        NEAREST,
        MIRROR,
        PERIODIC
    };
};

#endif // IMAGESTRATEGY_H
