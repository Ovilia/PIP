#include "BinaryMorphology.h"

const uchar BinaryMorphology::DEFAULT_BACK_COLOR = 0;
const uchar BinaryMorphology::DEFAULT_FORE_COLOR = 255;

BinaryMorphology::BinaryMorphology(ImageProcessor* imageProcessor,
                                   bool whiteAsForeground) :
    Morphology(imageProcessor->getBinaryImage())
{
    setForeground(whiteAsForeground);
}

BinaryMorphology::BinaryMorphology(QImage *binaryImage,
                                   bool whiteAsForeground) :
    Morphology(binaryImage)
{
    setForeground(whiteAsForeground);
}

BinaryMorphology::~BinaryMorphology()
{
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
