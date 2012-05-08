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
    ~BinaryMorphology();

    void doDilation(const StructElement& se);
    void doErosion(const StructElement& se);
    void doOpening(const StructElement& se);
    void doClosing(const StructElement& se);

    void setForeground(bool whiteAsForeground);

    QImage* getOperatedImage();

    // return if undo successfully
    bool undo();
    // return if redo successfully
    bool redo();
    // return if can undo
    bool canUndo();
    // return if can redo
    bool canRedo();

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
