#include "GrayMorphology.h"

GrayMorphology::GrayMorphology(ImageProcessor* imageProcessor) :
    Morphology(imageProcessor->getGrayScaleImage())
{
}

GrayMorphology::~GrayMorphology()
{

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
