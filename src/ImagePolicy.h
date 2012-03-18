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
        OTSU,
        ENTROPY,
        COSTUMED
    };
};

#endif // IMAGESTRATEGY_H
