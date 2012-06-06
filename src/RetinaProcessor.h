#ifndef RETINAPROCESSOR_H
#define RETINAPROCESSOR_H

#include "ImageProcessor.h"

class RetinaProcessor : public ImageProcessor
{
public:
    RetinaProcessor(QImage* originImage, QImage* maskImage);
    ~RetinaProcessor();

    QImage* getGrayScaleImage();
    int* getHistogram();
    QImage* getEqualImage();

private:
    int imageSize;
    bool* maskPtr;
    int maskSize;
};

#endif // RETINAPROCESSOR_H
