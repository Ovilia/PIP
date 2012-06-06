#ifndef GRAYMORPHOLOGY_H
#define GRAYMORPHOLOGY_H

#include "ImageProcessor.h"
#include "Morphology.h"

class GrayMorphology : public Morphology
{
public:
    GrayMorphology(ImageProcessor* imageProcessor);
    ~GrayMorphology();

private:
    static const int MAX_VALUE = 255;
    static const int MIN_VALUE = 0;

    virtual QImage* dilationHelper(const QImage& image,
                                   const StructElement& se) const;
    virtual QImage* erosionHelper(const QImage& image,
                                  const StructElement& se) const;
};

#endif // GRAYMORPHOLOGY_H
