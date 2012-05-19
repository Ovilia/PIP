#ifndef BinaryMorphology_H
#define BinaryMorphology_H

#include <QImage>

#include "ImageProcessor.h"
#include "StructElement.h"

class BinaryMorphology
{
public:
    BinaryMorphology(ImageProcessor* imageProcessor,
                     bool whiteAsForeground = true);
    BinaryMorphology(QImage* binaryImage,
                     bool whiteAsForeground = true);
    ~BinaryMorphology();

    // return operated image
    QImage* doDilation(const StructElement& se);
    QImage* doErosion(const StructElement& se);
    QImage* doOpening(const StructElement& se);
    QImage* doClosing(const StructElement& se);

    void setForeground(bool whiteAsForeground);
    uchar getForeground() const;
    uchar getBackground() const;

    QImage* getOperatedImage() const;

    // check if current image is of one color
    bool isOneColor(bool& isAllFore, bool& isAllBack) const;
    bool isAllFore() const;
    bool isAllBack() const;

    // return if undo successfully
    bool undo();
    // return if redo successfully
    bool redo();
    // return if can undo
    bool canUndo() const;
    // return if can redo
    bool canRedo() const;

    static const uchar DEFAULT_FORE_COLOR;
    static const uchar DEFAULT_BACK_COLOR;

private:
    uchar foreGroundColor, backGroundColor;

    static const int SE_MATCH = 1;

    static const int BUFFER_SIZE = 32;
    QImage* bufferImage[BUFFER_SIZE];
    int bufferCurrentIndex;
    int bufferUsedIndex;

    QImage* dilationHelper(const QImage& image, const StructElement& se);
    QImage* erosionHelper(const QImage& image, const StructElement& se);
    // push image to buffer
    void pushImage(QImage* image);
};

#endif // BinaryMorphology_H
