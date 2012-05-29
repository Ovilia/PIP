#ifndef BinaryMorphology_H
#define BinaryMorphology_H

#include <QImage>

#include "ImageProcessor.h"
#include "Morphology.h"

class BinaryMorphology : public Morphology
{
public:
    BinaryMorphology(ImageProcessor* imageProcessor,
                     bool whiteAsForeground = true);
    BinaryMorphology(QImage* binaryImage,
                     bool whiteAsForeground = true);
    ~BinaryMorphology();

    void setForeground(bool whiteAsForeground);
    uchar getForeground() const;
    uchar getBackground() const;

    // check if current image is of one color
    bool isOneColor(bool& isAllFore, bool& isAllBack) const;
    bool isAllFore() const;
    bool isAllBack() const;

    static const uchar DEFAULT_FORE_COLOR;
    static const uchar DEFAULT_BACK_COLOR;

private:
    uchar foreGroundColor, backGroundColor;

    virtual QImage* dilationHelper(const QImage& image,
                                   const StructElement& se);
    virtual QImage* erosionHelper(const QImage& image,
                                  const StructElement& se);
};

#endif // BinaryMorphology_H
