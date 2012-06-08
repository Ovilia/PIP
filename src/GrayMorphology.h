#ifndef GRAYMORPHOLOGY_H
#define GRAYMORPHOLOGY_H

#include "ImageProcessor.h"
#include "Morphology.h"

class GrayMorphology : public Morphology
{
public:
    GrayMorphology(ImageProcessor* imageProcessor);
    ~GrayMorphology();

    QImage* getReconstructImage(const StructElement& se,
                                const QImage& mask);

    QImage* getGradientImage(const StructElement& se, EdgeType edgeType);

private:
    static const int MAX_VALUE = 255;
    static const int MIN_VALUE = 0;

    virtual QImage* dilationHelper(const QImage& image,
                                   const StructElement& se) const;
    virtual QImage* erosionHelper(const QImage& image,
                                  const StructElement& se) const;

    // (left - right) / 2
    QImage* halfMinusHelper(const QImage& left, const QImage& right) const;

    QImage* reconstruct;
    QImage* gradient;
};

#endif // GRAYMORPHOLOGY_H
