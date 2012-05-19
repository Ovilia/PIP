#include "BinaryMorphology.h"

const uchar BinaryMorphology::DEFAULT_BACK_COLOR = 0;
const uchar BinaryMorphology::DEFAULT_FORE_COLOR = 255;

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

BinaryMorphology::BinaryMorphology(QImage *binaryImage,
                                   bool whiteAsForeground) :
    bufferCurrentIndex(0),
    bufferUsedIndex(0)
{
    bufferImage[0] = binaryImage;
    for (int i = 1; i < BUFFER_SIZE; ++i) {
        bufferImage[i] = 0;
    }
    setForeground(whiteAsForeground);
}

BinaryMorphology::~BinaryMorphology()
{
    // i start from 1 so that won't delete original binary image
    for (int i = 1; i < BUFFER_SIZE; ++i) {
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

QImage* BinaryMorphology::doDilation(const StructElement& se)
{
    QImage* image = dilationHelper(*bufferImage[bufferCurrentIndex], se);
    pushImage(image);
    return image;
}

QImage* BinaryMorphology::doErosion(const StructElement& se)
{
    QImage* image = erosionHelper(*bufferImage[bufferCurrentIndex], se);
    pushImage(image);
    return image;
}

QImage* BinaryMorphology::doOpening(const StructElement& se)
{
    QImage* imageEro = erosionHelper(*bufferImage[bufferCurrentIndex], se);
    QImage* imageOpen = dilationHelper(*imageEro, se);
    delete imageEro;
    pushImage(imageOpen);
    return imageOpen;
}

QImage* BinaryMorphology::doClosing(const StructElement& se)
{
    QImage* imageDil = dilationHelper(*bufferImage[bufferCurrentIndex], se);
    QImage* imageClose = erosionHelper(*imageDil, se);
    delete imageDil;
    pushImage(imageClose);
    return imageClose;
}

QImage* BinaryMorphology::dilationHelper(const QImage& image,
                                         const StructElement& se)
{
    int width = image.width();
    int height = image.height();
    int size = width * height;
    int totalBits = width * height * 4;
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
                            if (index >= 0 && index < totalBits) {
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
    int width = image.width();
    int height = image.height();
    QImage* newImage = new QImage(image.size(), image.format());

    // erosion
    int minX = se.getMinX();
    int maxX = se.getMaxX();
    int minY = se.getMinY();
    int maxY = se.getMaxY();
    int totalBits = width * height * 4;
    uchar* nBits = newImage->bits();
    const uchar* oBits = image.constBits();

    for (int h = 0; h < height; ++h){
        for (int w = 0; w < width; ++w) {
            bool isFore = true;
            for (int x = minX; isFore && x < maxX; ++x) {
                for (int y = minY; isFore && y < maxY; ++y) {
                    int index = ((h + y) * width + (w + x)) * 4;
                    // when se match but not foreground in image, not hit
                    if (index < 0 || index > totalBits ||
                            (se.getValue(x, y) == SE_MATCH &&
                                      *(oBits + index) != foreGroundColor)) {
                        isFore = false;
                    }
                }
            }
            int color = backGroundColor;
            if (isFore) {
                color = foreGroundColor;
            }
            for (int rgb = 0; rgb < 3; ++rgb) {
                *(nBits + rgb) = color;
            }
            nBits += 4;
        }
    }
    return newImage;
}

QImage* BinaryMorphology::getOperatedImage() const
{
    return bufferImage[bufferCurrentIndex];
}

bool BinaryMorphology::undo()
{
    if (bufferCurrentIndex > 0) {
        --bufferCurrentIndex;
        return true;
    } else {
        return false;
    }
}

bool BinaryMorphology::redo()
{
    if (bufferUsedIndex > bufferCurrentIndex) {
        ++bufferCurrentIndex;
        return true;
    } else {
        return false;
    }
}

bool BinaryMorphology::canUndo() const
{
    if (bufferCurrentIndex > 0) {
        return true;
    } else {
        return false;
    }
}

bool BinaryMorphology::canRedo() const
{
    if (bufferUsedIndex > bufferCurrentIndex) {
        return true;
    } else {
        return false;
    }
}

void BinaryMorphology::pushImage(QImage *image)
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

bool BinaryMorphology::isOneColor(bool& isAllFore, bool& isAllBack) const
{
    QImage* image = getOperatedImage();
    int size = image->width() * image->height();
    const uchar* bits = image->constBits();
    isAllFore = true;
    isAllBack = true;
    for (int i = 0; i < size; ++i) {
        if (*bits == backGroundColor) {
            // back
            isAllFore = false;
        } else {
            // fore
            isAllBack = false;
        }
        // next pixel
        bits += 4;
    }
    return isAllFore || isAllBack;
}

bool BinaryMorphology::isAllBack() const
{
    QImage* image = getOperatedImage();
    int size = image->width() * image->height();
    const uchar* bits = image->constBits();
    for (int i = 0; i < size; ++i) {
        if (*bits != backGroundColor) {
            return false;
        }
        // next pixel
        bits += 4;
    }
    return true;
}

bool BinaryMorphology::isAllFore() const
{
    QImage* image = getOperatedImage();
    int size = image->width() * image->height();
    const uchar* bits = image->constBits();
    for (int i = 0; i < size; ++i) {
        if (*bits != foreGroundColor) {
            return false;
        }
        // next pixel
        bits += 4;
    }
    return true;
}

uchar BinaryMorphology::getForeground() const
{
    return foreGroundColor;
}

uchar BinaryMorphology::getBackground() const
{
    return backGroundColor;
}
