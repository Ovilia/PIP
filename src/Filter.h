#ifndef FILTER_H
#define FILTER_H

#include <QImage>

#include "ImagePolicy.h"

class Filter
{
public:
    /**
     * kernelRadio is the border radio of kernel
     * for a 3x3 kernel, kernelRadio is 1
     * for a 5x5 kernel, kernelRadion is 2
     */
    Filter(QImage* image, int kernelRadio,
           ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    virtual ~Filter();

    void setBorderPolicy(ImagePolicy::BorderPolicy policy);

    /**
     * this is not a virtual function itself
     * but it will call virtual function doFiltering
     */
    QImage* getFilteredImage();

protected:
    QImage* originImage;
    QImage* filteredImage;

    int kernelRadio;

    ImagePolicy::BorderPolicy borderPolicy;
    /**
     * if border policy changed after last filtering image
     * if isBorderChanged and filteredImage is not null, recalculate the
     * border part instead of recalculating the whole image
     */
    bool isBorderChanged;

    enum ColorOffset {
        // offset of red pixel in QImage->bits()
        RED_OFFSET = 2,
        GREEN_OFFSET = 1,
        BLUE_OFFSET = 0
    };

    // size of each pixel
    static const int PIXEL_SIZE = 4;
    // max value of a bit
    static const uchar MAX_BIT_VALUE = 255;

    /**
     * proceed filtering
     * @param x x index in originImage
     * @param y y index in originImage
     * @param offset ColorOffset of the wanted pixel
     * @return filtered color
     */
    virtual uchar doFiltering(int x, int y, ColorOffset offset) = 0;

    /**
     * get 1-dimension index with given 2-dimension index
     * @param columns first dimension width of 2-dimension data
     * @param x x index of 2-dimension data
     * @param y y index of 2-dimension data
     * @return index of 1-dimension data
     */
    int get2DIndex(int columns, int x, int y);

    /**
     * get red pixel value in image
     * @param x ranging from -kernelRadio to image->width() - 1 + kernelRadio
     * @param y ranging from -kernelRadio to image->height() - 1 + kernelRadio
     * @param offset ColorOffset of the wanted pixel
     * @return pixel value of image, if x is out of the range
               [0, image->width() - 1] or y is out of the range
               [0, image->height() - 1], it returns the border value
               processed using borderPolicy.
     */
    const uchar getBorderedValue(int x, int y, ColorOffset offset);

    /**
     * helper function for getBorderedValue
     * if under current borderPolicy, every (x, y) pair is corresponded to
     * a pixel in originImage, return the amount of pixels which is before it
     * otherwise (e.g. borderPolicy == ImagePolicy::BLACK),
     * this function isn't supposed to be called and -1 is returned
     */
    inline const int getBorderedIndex(int x, int y);

    /**
     * 9 parts divided by border
     * +-----------------+-----------------+-----------------+
     * |TOP_LEFT         |TOP_CENTER       |TOP_RIGHT        |
     * +-----------------+-----------------+-----------------+---- border
     * |CENTER_LEFT      |CENTER_CENTER    |CENTER_RIGHT     |
     * |                 |(origin image)   |                 |
     * +-----------------+-----------------+-----------------+---- border
     * |BOTTOM_LEFT      |BOTTOM_CENTER    |BOTTOM_RIGHT     |
     * +-----------------+-----------------+-----------------+
     *                   |                 |
     *                   |                 |
     *                 border            border
     */
    enum BorderedPart {
        TOP_LEFT = 0,
        TOP_CENTER,
        TOP_RIGHT,
        CENTER_LEFT,
        CENTER_CENTER,
        CENTER_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT,
        ILLEGAL_PART = -1
    };
    /**
     * get part index of image, if x or y is out of range,
     * ILLEGAL_PART is returned
     * @param x [-kernelRadio, image->width() - 1]
     * @param y [-kernelRadio, image->height() - 1]
     */
    inline BorderedPart getBorderedPart(int x, int y);

    /**
     * change pixel data when setBorderPolicy is called
     * if filteredImage is null, do nothing
     */
    void resetBorderPixel();

    /**
     * change all pixel data of filteredImage
     */
    void resetAllPixel();
};

#endif // FILTER_H
