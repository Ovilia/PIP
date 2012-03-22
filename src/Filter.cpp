#include "Filter.h"

Filter::Filter(QImage* image, const int kernelRadio,
               ImagePolicy::BorderPolicy policy) :
    originImage(image),
    filteredImage(0),
    kernelRadio(kernelRadio),
    borderPolicy(policy),
    isBorderChanged(false),
    isKernelChanged(false)
{
}

Filter::~Filter()
{
    if (filteredImage) {
        delete filteredImage;
    }
}

void Filter::setBorderPolicy(ImagePolicy::BorderPolicy policy)
{
    if (policy != borderPolicy) {
        borderPolicy = policy;
        isBorderChanged = true;
    }
}

int Filter::get2DIndex(int columns, int x, int y)
{
    return columns * y + x;
}

const uchar Filter::getBorderedValue(int x, int y, ColorOffset offset)
{
    if (borderPolicy == ImagePolicy::BLACK) {
        return 0;
    }
    return *(originImage->constBits() +
             PIXEL_SIZE * getBorderedIndex(x, y)
             + offset);
}

inline const int Filter::getBorderedIndex(int x, int y)
{
    Filter::BorderedPart part = getBorderedPart(x, y);
    int width = originImage->size().width();
    int height = originImage->size().height();

    switch (borderPolicy) {
    case ImagePolicy::NEAREST:
        switch (part) {
        case Filter::TOP_LEFT:
            return get2DIndex(width, 0, 0);
        case Filter::TOP_CENTER:
            return get2DIndex(width, x, 0);
        case Filter::TOP_RIGHT:
            return get2DIndex(width, width - 1, 0);
        case Filter::CENTER_LEFT:
            return get2DIndex(width, 0, y);
        case Filter::CENTER_CENTER:
            return get2DIndex(width, x, y);
        case Filter::CENTER_RIGHT:
            return get2DIndex(width, width - 1, y);
        case Filter::BOTTOM_LEFT:
            return get2DIndex(width, 0, height - 1);
        case Filter::BOTTOM_CENTER:
            return get2DIndex(width, x, height - 1);
        case Filter::BOTTOM_RIGHT:
            return get2DIndex(width, width - 1, height - 1);
        case Filter::ILLEGAL_PART:
            return -1;
        }

    case ImagePolicy::MIRROR:
        switch (part) {
        case Filter::TOP_LEFT:
            return get2DIndex(width, - x - 1, - y - 1);
        case Filter::TOP_CENTER:
            return get2DIndex(width, x, - y - 1);
        case Filter::TOP_RIGHT:
            return get2DIndex(width, 2 * width - 1 - x, - y - 1);
        case Filter::CENTER_LEFT:
            return get2DIndex(width, - x - 1, y);
        case Filter::CENTER_CENTER:
            return get2DIndex(width, x, y);
        case Filter::CENTER_RIGHT:
            return get2DIndex(width, 2 * width - x - 1, y);
        case Filter::BOTTOM_LEFT:
            return get2DIndex(width, - x - 1, 2 * height - 1 - y);
        case Filter::BOTTOM_CENTER:
            return get2DIndex(width, x, 2 * height - y - 1);
        case Filter::BOTTOM_RIGHT:
            return get2DIndex(width, 2 * width - 1 - x, 2 * height - 1 - y);
        case Filter::ILLEGAL_PART:
            return -1;
        }

    case ImagePolicy::PERIODIC:
        switch (part) {
        case Filter::TOP_LEFT:
            return get2DIndex(width, x + width, y + height);
        case Filter::TOP_CENTER:
            return get2DIndex(width, x, y + height);
        case Filter::TOP_RIGHT:
            return get2DIndex(width, x - width, y + height);
        case Filter::CENTER_LEFT:
            return get2DIndex(width, x + width, y);
        case Filter::CENTER_CENTER:
            return get2DIndex(width, x, y);
        case Filter::CENTER_RIGHT:
            return get2DIndex(width, x - width, y);
        case Filter::BOTTOM_LEFT:
            return get2DIndex(width, x + width, y - height);
        case Filter::BOTTOM_CENTER:
            return get2DIndex(width, x, y - height);
        case Filter::BOTTOM_RIGHT:
            return get2DIndex(width, x - width, y - height);
        case Filter::ILLEGAL_PART:
            return -1;
        }

    case ImagePolicy::BLACK:
        return -1;
    }
}

inline Filter::BorderedPart Filter::getBorderedPart(int x, int y)
{
    int width = originImage->width();
    int height = originImage->height();
    if (x >= -kernelRadio && x <0) {
        // left part
        if (y >= -kernelRadio && y < 0) {
            return TOP_LEFT;
        } else if (y >= 0 && y < height) {
            return CENTER_LEFT;
        } else if (y >= height && y < height + kernelRadio) {
            return BOTTOM_LEFT;
        } else {
            return ILLEGAL_PART;
        }
    } else if (x > 0 && x < width) {
        // center
        if (y >= -kernelRadio && y < 0) {
            return TOP_CENTER;
        } else if (y >= 0 && y < height) {
            return CENTER_CENTER;
        } else if (y >= height && y < height + kernelRadio) {
            return BOTTOM_CENTER;
        } else {
            return ILLEGAL_PART;
        }
    } else if (x >= width && x < width + kernelRadio) {
        // right
        if (y >= -kernelRadio && y < 0) {
            return TOP_RIGHT;
        } else if (y >= 0 && y < height) {
            return CENTER_RIGHT;
        } else if (y >= height && y < height + kernelRadio) {
            return BOTTOM_RIGHT;
        } else {
            return ILLEGAL_PART;
        }
    } else {
        return ILLEGAL_PART;
    }
}

void Filter::resetBorderPixel()
{
    if (!filteredImage) {
        int width = originImage->size().width();
        int height = originImage->size().height();

        // only calculate pixels that will change with borderPolicy
        // x -> [0, kernelRadio] || [width - kernelRadio, width - 1]
        // y -> [0, kernelRadio] || [height - kernelRadio, height - 1]
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                if (x < kernelRadio || x > width - kernelRadio ||
                        y < kernelRadio || y > height - kernelRadio) {
                    // BLUE_OFFSET is used here since rgb is the same
                    // and blue is the first pixel
                    uchar gray = doFiltering(x, y, BLUE_OFFSET);
                    uchar* bits = filteredImage->bits()
                            + get2DIndex(width, x, y);
                    // set rgb
                    *bits = gray;
                    *(bits + 1) = gray;
                    *(bits + 2) = gray;
                }
            }
        }
    }
    // set isBorderChanged flag to be false
    isBorderChanged = false;
}

void Filter::resetAllPixel()
{
    if (!filteredImage) {
        uchar* bits = filteredImage->bits();
        int width = originImage->size().width();
        int height = originImage->size().height();
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                uchar gray = doFiltering(x, y, BLUE_OFFSET);
                *bits = gray;
                *(bits + 1) = gray;
                *(bits + 2) = gray;
                // move to next pixel
                bits += PIXEL_SIZE;
            }
        }
    }
    // don't calculate again
    isKernelChanged = false;
    isBorderChanged = false;
}

QImage* Filter::getFilteredImage()
{
    if (filteredImage == 0) {
        // init filteredImage and reset all
        int width = originImage->width();
        int height = originImage->height();
        filteredImage = new QImage(width, height, originImage->format());
        resetAllPixel();
    } else if (isKernelChanged) {
        // reset if kernel is changed
        resetAllPixel();
    } else if (isBorderChanged) {
        /**
         * if previously calculated center image and border policy changed,
         * only recalculate the border part
         */
        resetBorderPixel();
    }
    return filteredImage;
}
