#ifndef MORPHODISTANCE_H
#define MORPHODISTANCE_H

#include <QImage>

#include "StructElement.h"
#include "ImageProcessor.h"

class MorphoDistance
{
public:
    MorphoDistance(ImageProcessor* imageProcessor);
    ~MorphoDistance();

    QImage* getDistanceImage(const bool useSquareSe = true);

    QImage* getSkeletonImage();

    bool isUseSquareSe() const;

private:
    ImageProcessor* imageProcessor;

    static const int SE_RATIO = 3;
    static const int SE_SQUARE_ARR[SE_RATIO * SE_RATIO];
    static const int SE_CROSS_ARR[SE_RATIO * SE_RATIO];

    QImage* distanceImage;

    QImage* skeletonImage;

    bool useSquareSe;
};

#endif // MORPHODISTANCE_H
