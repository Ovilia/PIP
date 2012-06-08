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

    enum EdgeType {
        ET_STANDARD,
        ET_INTERNAL,
        ET_EXTERNAL
    };

protected:
    static const int BUFFER_SIZE = 32;
    QImage* bufferImage[BUFFER_SIZE];
    int bufferCurrentIndex;
    int bufferUsedIndex;

    virtual QImage* dilationHelper(const QImage& image,
                                   const StructElement& se) const = 0;
    virtual QImage* erosionHelper(const QImage& image,
                                  const StructElement& se) const = 0;
    QImage* openingHelper(const QImage& image,
                          const StructElement& se) const;
    QImage* closingHelper(const QImage& image,
                          const StructElement& se) const;

    // push image to buffer
    void pushImage(QImage* image);

    bool sameImage(const QImage& left, const QImage& right) const;
};

#endif // MORPHOLOGY_H
