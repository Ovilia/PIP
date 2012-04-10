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

    enum ScalePolicy {
        SP_NEAREST = 0,
        SP_BILINEAR
    };

    enum AlgebraPolicy {
        AP_ADD = 0,
        AP_MINUS,
        AP_MULTIPLY,
        AP_DIVIDE
    };
};

#endif // IMAGESTRATEGY_H
