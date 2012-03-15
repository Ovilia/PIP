#ifndef IMAGESTRATEGY_H
#define IMAGESTRATEGY_H

class ImageStrategy
{
public:
    enum GrayScaleStrategy {
        MATCH_LUMINANCE = 0,
        RGB_AVERAGE,
        GREEN_ONLY
    };
};

#endif // IMAGESTRATEGY_H
