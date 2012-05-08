#include "BinaryMorphology.h"

BinaryMorphology::BinaryMorphology(ImageProcessor* imageProcessor,
                                 bool whiteAsForeground) :
    bufferCurrentIndex(0),
    bufferUsedIndex(0)
{
    bufferImage[0] = imageProcessor->getBinaryImage();
    for (int i = 1; i < BUFFER_SIZE; ++i) {
        bufferImage[i] = 0;
    }
    setForeground(whiteAsForeground);
}

BinaryMorphology::~BinaryMorphology()
{
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (bufferImage[i]) {
            delete bufferImage[i];
        }
    }
}

void BinaryMorphology::setForeground(bool whiteAsForeground)
{
    if (whiteAsForeground) {
        foreGroundColor = 255;
        backGroundColor = 0;
    } else {
        foreGroundColor = 0;
        backGroundColor = 255;
    }
}

void BinaryMorphology::doDilation(const StructElement& se)
{
    QImage* image = dilationHelper(*bufferImage[bufferCurrentIndex], se);
    pushImage(image);
}

void BinaryMorphology::doErosion(const StructElement& se)
{
    QImage* image = erosionHelper(*bufferImage[bufferCurrentIndex], se);
    pushImage(image);
}

void BinaryMorphology::doOpening(const StructElement& se)
{
    QImage* imageEro = erosionHelper(*bufferImage[bufferCurrentIndex], se);
    QImage* imageOpen = dilationHelper(*imageEro, se);
    delete imageEro;
    pushImage(imageOpen);
}

void BinaryMorphology::doClosing(const StructElement& se)
{
    QImage* imageDil = dilationHelper(*bufferImage[bufferCurrentIndex], se);
    QImage* imageClose = erosionHelper(*imageDil, se);
    delete imageDil;
    pushImage(imageClose);
}

QImage* BinaryMorphology::dilationHelper(const QImage& image,
                                         const StructElement& se)
{
    int width = image.width();
    int height = image.height();
    int size = width * height;
    QImage* newImage = new QImage(image.size(), image.format());
    uchar* nBits = newImage->bits();

    // set all pixel to background
    for (int i = 0; i < size; ++i) {
        for (int rgb = 0; rgb < 3; ++rgb) {
            *nBits = backGroundColor;
            ++nBits;
        }
        ++nBits;
    }

    // dilation
    int minX = se.getMinX();
    int maxX = se.getMaxX();
    int minY = se.getMinY();
    int maxY = se.getMaxY();
    const uchar* oBits = image.constBits();
    nBits = newImage->bits();
    for (int h = 0; h < height; ++h){
        for (int w = 0; w < width; ++w) {
            // rgb of binary image is the same, compare blue only
            if (*oBits == foreGroundColor) {
                for (int x = minX; x < maxX; ++x) {
                    for (int y = minY; y < maxY; ++y) {
                        if (se.getValue(x, y) == SE_MATCH) {
                            // set rgb to be foregound
                            int index = ((h + y) * width + (w + x)) * 4;
                            if (index > 0) {
                                for (int rgb = 0; rgb < 3; ++rgb) {
                                    *(nBits + index + rgb) = foreGroundColor;
                                }
                            }
                        }
                    }
                }
            }
            // to next pixel
            oBits += 4;
        }
    }

    return newImage;
}

QImage* BinaryMorphology::erosionHelper(const QImage& image,
                                        const StructElement& se)
{

}

QImage* BinaryMorphology::getOperatedImage()
{
    return bufferImage[bufferCurrentIndex];
}

bool BinaryMorphology::undo()
{
    if (bufferUsedIndex > bufferCurrentIndex) {
        ++bufferCurrentIndex;
        return true;
    } else {
        return false;
    }
}

bool BinaryMorphology::redo()
{
    if (bufferCurrentIndex > 0) {
        --bufferCurrentIndex;
        return true;
    } else {
        return false;
    }
}

bool BinaryMorphology::canUndo()
{
    if (bufferUsedIndex > bufferCurrentIndex) {
        return true;
    } else {
        return false;
    }
}

bool BinaryMorphology::canRedo()
{
    if (bufferCurrentIndex > 0) {
        return true;
    } else {
        return false;
    }
}

void BinaryMorphology::pushImage(QImage *image)
{
    if (bufferUsedIndex < BUFFER_SIZE - 1) {
        // buffer is not full, push to the end
        ++bufferCurrentIndex;
        bufferImage[bufferCurrentIndex] = image;
        bufferUsedIndex = bufferCurrentIndex;
    } else {
        // buffer is full, delete the oldest one
        for (int i = 1; i < BUFFER_SIZE; ++i) {
            bufferImage[i - 1] = bufferImage[i];
        }
        bufferImage[BUFFER_SIZE - 1] = image;
    }
}
