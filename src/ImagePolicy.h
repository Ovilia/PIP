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
        NEAREST = 0,
        MIRROR,
        PERIODIC,
        BLACK
    };
};

#endif // IMAGESTRATEGY_H
