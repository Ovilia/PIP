#include "Morphology.h"

Morphology::Morphology(QImage *originImage) :
    bufferCurrentIndex(0),
    bufferUsedIndex(0)
{
    bufferImage[0] = originImage;
    for (int i = 1; i < BUFFER_SIZE; ++i) {
        bufferImage[i] = 0;
    }
}

Morphology::~Morphology()
{
    // i start from 1 so that won't delete original binary image
    for (int i = 1; i < BUFFER_SIZE; ++i) {
        if (bufferImage[i]) {
            delete bufferImage[i];
        }
    }
}

QImage* Morphology::doDilation(const StructElement& se)
{
    QImage* image = dilationHelper(*bufferImage[bufferCurrentIndex], se);
    pushImage(image);
    return image;
}

QImage* Morphology::doErosion(const StructElement& se)
{
    QImage* image = erosionHelper(*bufferImage[bufferCurrentIndex], se);
    pushImage(image);
    return image;
}

QImage* Morphology::doOpening(const StructElement& se)
{
    QImage* image = openingHelper(*bufferImage[bufferCurrentIndex], se);
    pushImage(image);
    return image;
}

QImage* Morphology::doClosing(const StructElement& se)
{
    QImage* image = closingHelper(*bufferImage[bufferCurrentIndex], se);
    pushImage(image);
    return image;
}

QImage* Morphology::openingHelper(const QImage& image,
                                  const StructElement& se) const
{
    QImage* imageEro = erosionHelper(image, se);
    QImage* imageOpen = dilationHelper(*imageEro, se);
    delete imageEro;
    return imageOpen;
}

QImage* Morphology::closingHelper(const QImage& image,
                                  const StructElement& se) const
{
    QImage* imageDil = dilationHelper(image, se);
    QImage* imageClose = erosionHelper(*imageDil, se);
    delete imageDil;
    return imageClose;
}

QImage* Morphology::getOperatedImage() const
{
    return bufferImage[bufferCurrentIndex];
}

bool Morphology::undo()
{
    if (bufferCurrentIndex > 0) {
        --bufferCurrentIndex;
        return true;
    } else {
        return false;
    }
}

bool Morphology::redo()
{
    if (bufferUsedIndex > bufferCurrentIndex) {
        ++bufferCurrentIndex;
        return true;
    } else {
        return false;
    }
}

bool Morphology::canUndo() const
{
    if (bufferCurrentIndex > 0) {
        return true;
    } else {
        return false;
    }
}

bool Morphology::canRedo() const
{
    if (bufferUsedIndex > bufferCurrentIndex) {
        return true;
    } else {
        return false;
    }
}

void Morphology::pushImage(QImage *image)
{
    if (bufferCurrentIndex < BUFFER_SIZE - 1) {
        // buffer is not full, push to the end
        ++bufferCurrentIndex;
        bufferUsedIndex = bufferCurrentIndex;
        bufferImage[bufferCurrentIndex] = image;
    } else {
        // buffer is full, delete the oldest one
        for (int i = 1; i < BUFFER_SIZE; ++i) {
            bufferImage[i - 1] = bufferImage[i];
        }
        bufferUsedIndex = bufferCurrentIndex;
        bufferImage[bufferCurrentIndex] = image;
    }
}

bool Morphology::sameImage(const QImage& left,
                           const QImage& right) const
{
    if (left.width() != right.width() || left.height() != right.height()) {
        return false;
    }
    int size = left.width() * left.height();
    const uchar* lBits = left.constBits();
    const uchar* rBits = right.constBits();
    for (int i = 0; i < size; ++i) {
        if (*lBits != *rBits) {
            return false;
        }
        lBits += 4;
        rBits +=4;
    }
    return true;
}
