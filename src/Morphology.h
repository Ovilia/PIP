#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

#include <QImage>

#include "StructElement.h"

class Morphology
{
public:
    Morphology(QImage* originImage);
    virtual ~Morphology();

    // return operated image
    QImage* doDilation(const StructElement& se);
    QImage* doErosion(const StructElement& se);
    QImage* doOpening(const StructElement& se);
    QImage* doClosing(const StructElement& se);

    QImage* getOperatedImage() const;

    // return if undo successfully
    bool undo();
    // return if redo successfully
    bool redo();
    // return if can undo
    bool canUndo() const;
    // return if can redo
    bool canRedo() const;
    static const int SE_MATCH = 1;

private:
    static const int BUFFER_SIZE = 32;
    QImage* bufferImage[BUFFER_SIZE];
    int bufferCurrentIndex;
    int bufferUsedIndex;

    virtual QImage* dilationHelper(const QImage& image,
                                   const StructElement& se) = 0;
    virtual QImage* erosionHelper(const QImage& image,
                                  const StructElement& se) = 0;
    // push image to buffer
    void pushImage(QImage* image);
};

#endif // MORPHOLOGY_H
