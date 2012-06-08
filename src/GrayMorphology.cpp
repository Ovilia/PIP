#include "GrayMorphology.h"

#include "ImageScaler.h"

GrayMorphology::GrayMorphology(ImageProcessor* imageProcessor) :
    Morphology(imageProcessor->getGrayScaleImage()),
    reconstruct(0),
    gradient(0)
{
}

GrayMorphology::~GrayMorphology()
{
    if (reconstruct) {
        delete reconstruct;
    }
    if (gradient) {
        delete gradient;
    }
}

QImage* GrayMorphology::dilationHelper(const QImage &image,
                                       const StructElement &se) const
{
    int width = image.width();
    int height = image.height();
    QImage* newImage = new QImage(image.size(), image.format());

    int minX = se.getMinX();
    int maxX = se.getMaxX();
    int minY = se.getMinY();
    int maxY = se.getMaxY();
    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            int max = qRed(image.pixel(w, h)) + se.getValue(0, 0);
            if (max > MAX_VALUE) {
                max = MAX_VALUE;
            } else if (max < MIN_VALUE) {
                max = MIN_VALUE;
            }
            for (int x = minX; x < maxX; ++x) {
                for (int y = minY; y < maxY; ++y) {
                    if (w + x >= 0 && w + x < width &&
                            h + y >= 0 && h + y < height) {
                        int gray = qRed(image.pixel(w + x, h + y)) +
                                se.getValue(x, y);
                        if (gray > max) {
                            max = gray;
                            if (max > MAX_VALUE) {
                                max = MAX_VALUE;
                            } else if (max < MIN_VALUE) {
                                max = MIN_VALUE;
                            }
                        }
                    }
                }
            }
            newImage->setPixel(w, h, qRgb(max, max, max));
        }
    }
    return newImage;
}

QImage* GrayMorphology::erosionHelper(const QImage& image,
                                      const StructElement& se) const
{
    int width = image.width();
    int height = image.height();
    QImage* newImage = new QImage(image.size(), image.format());

    int minX = se.getMinX();
    int maxX = se.getMaxX();
    int minY = se.getMinY();
    int maxY = se.getMaxY();
    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            int min = qRed(image.pixel(w, h)) - se.getValue(0, 0);
            if (min > MAX_VALUE) {
                min = MAX_VALUE;
            } else if (min < MIN_VALUE) {
                min = MIN_VALUE;
            }
            for (int x = minX; x < maxX; ++x) {
                for (int y = minY; y < maxY; ++y) {
                    if (w + x >= 0 && w + x < width &&
                            h + y >= 0 && h + y < height) {
                        int gray = qRed(image.pixel(w + x, h + y));
                        if (gray < min) {
                            min = gray;
                            if (min > MAX_VALUE) {
                                min = MAX_VALUE;
                            } else if (min < MIN_VALUE) {
                                min = MIN_VALUE;
                            }
                        }
                    }
                }
            }
            newImage->setPixel(w, h, qRgb(min, min, min));
        }
    }
    return newImage;
}

QImage* GrayMorphology::getReconstructImage(const StructElement& se,
                                            const QImage& mask)
{
    if (reconstruct) {
        delete reconstruct;
    }

    QImage* last = getOperatedImage();
    QImage* scaledMask;
    if (mask.width() == last->width() && mask.height() == last->height()) {
        scaledMask = new QImage(mask);
    } else {
        scaledMask = ImageScaler::getScaledImage(
                    &mask, last->width(),last->height(),
                    ImagePolicy::SP_NEAREST);
    }

    int size = last->width() * last->height();
    bool isFirst = true;
    while (true) {
        QImage* dilation = dilationHelper(*last, se);

        uchar* dBits = dilation->bits();
        const uchar* mBits = scaledMask->constBits();
        for (int i = 0; i < size; ++i) {
            if (*dBits > *mBits) {
                for (int rgb = 0; rgb < 3; ++rgb) {
                    *(dBits + rgb) = *mBits;
                }
            }
            dBits += 4;
            mBits += 4;
        }

        // check if is stable
        if (sameImage(*last, *dilation)) {
            reconstruct = dilation;
            break;
        }
        if (!isFirst) {
            // not to delete origin image
            delete last;
        }
        isFirst = false;
        last = dilation;
    }
    return reconstruct;
}

QImage* GrayMorphology::getGradientImage(const StructElement& se,
                                         EdgeType edgeType)
{
    if (gradient) {
        delete gradient;
    }
    QImage* origin = getOperatedImage();

    switch (edgeType) {
    case ET_STANDARD:
    {
        QImage* dilation = dilationHelper(*origin, se);
        QImage* erotion = erosionHelper(*origin, se);
        gradient = halfMinusHelper(*dilation, *erotion);
        delete dilation;
        delete erotion;
    }
        break;

    case ET_INTERNAL:
    {
        QImage* erotion = erosionHelper(*origin, se);
        gradient = halfMinusHelper(*origin, *erotion);
        delete erotion;
    }
        break;

    case ET_EXTERNAL:
    {
        QImage* dilation = dilationHelper(*origin, se);
        gradient = halfMinusHelper(*dilation, *origin);
        delete dilation;
    }
        break;
    }

    return gradient;
}

QImage* GrayMorphology::halfMinusHelper(const QImage &left,
                                        const QImage &right) const
{
    if (left.width() != right.width() || left.height() != right.height()) {
        return 0;
    }
    int size = left.width() * left.height();
    const uchar* lBits = left.constBits();
    const uchar* rBits = right.constBits();
    QImage* result = new QImage(left.size(), left.format());
    uchar* bits = result->bits();
    for (int i = 0; i < size; ++i) {
        int value = (*lBits - *rBits) / 2;
        if (value > 255) {
            value = 255;
        } else if (value < 0) {
            value = 0;
        }
        for (int rgb = 0; rgb < 3; ++rgb) {
            *(bits + rgb) = value;
        }
        *(bits + 4) = 255;
        lBits += 4;
        rBits +=4;
        bits += 4;
    }
    return result;
}
