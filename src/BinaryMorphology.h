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

    static const int SE_NOT_MATCH = 0;

    enum EdgeType {
        ET_STANDARD,
        ET_INTERNAL,
        ET_EXTERNAL
    };

    QImage* getEdgeImage(const StructElement& se, EdgeType edgeType);
    QImage* getReconstructImage(const StructElement& se);

private:
    uchar foreGroundColor, backGroundColor;

    virtual QImage* dilationHelper(const QImage& image,
                                   const StructElement& se) const;
    virtual QImage* erosionHelper(const QImage& image,
                                  const StructElement& se) const;

    QImage* minusHelper(const QImage& left, const QImage& right) const;

    QImage* edgeImage;
    QImage* reconstructImage;
};

#endif // BinaryMorphology_H
