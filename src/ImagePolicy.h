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
};

#endif // IMAGESTRATEGY_H
