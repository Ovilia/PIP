#ifndef RETINAPROCESSOR_H
#define RETINAPROCESSOR_H

#include "BinaryMorphology.h"
#include "ImageProcessor.h"

class RetinaProcessor : public ImageProcessor
{
public:
    RetinaProcessor(QImage* originImage, QImage* maskImage);
    ~RetinaProcessor();

    QImage* getGrayScaleImage();
    int* getHistogram();
    QImage* getEqualImage();

    QImage* getRetinaImage();
    QImage* getCenterImage();

private:
    QImage* maskImage;
    int imageSize;
    bool* maskPtr;
    int maskSize;

    QImage* retinaImage;
    QImage* centerImage;

    BinaryMorphology* morpho;
};

#endif // RETINAPROCESSOR_H
