#ifndef FILTER_H
#define FILTER_H

#include <QImage>

class Filter
{
public:
    /**
     * kernelRadio is the border radio of kernel
     * for a 3x3 kernel, kernelRadio is 1
     * for a 5x5 kernel, kernelRadion is 2
     */
    Filter(QImage* image, int* kernelPtr, int kernelRadio);
    Filter(QImage* image, double* kernelPtr, int kernelRadio);
    ~Filter();

    virtual QImage* getFilteredImage() = 0;

protected:
    QImage* originImage;
    int* kernelIntPtr;
    double* kernelDoublePtr;
    int kernelRadio;

    /**
     * get 1-dimension index with given 2-dimension index
     * @param columns first dimension width of 2-dimension data
     * @param x x index of 2-dimension data
     * @param y y index of 2-dimension data
     * @return index of 1-dimension data
     */
    inline int get2DIndex(int columns, int x, int y);

    /**
     * get red pixel value in image
     * @param x ranging from -kernelRadio to image->width() + kernelRadio
     * @param y ranging from -kernelRadio to image->width() + kernelRadio
     * @return red value of image, if x or y is out of the
               range [0, image->width()], it returns the border value
               processed using borderPolicy.
               -1 is returned if it is originally initialized with double
     */
    inline const int getBorderedRedInt(int x, int y);
    inline const int getBorderedGreenInt(int x, int y);
    inline const int getBorderedBlueInt(int x, int y);
    inline const double getBorderedRedDouble(int x, int y);
    inline const double getBorderedRedDouble(int x, int y);
    inline const double getBorderedRedDouble(int x, int y);
};

#endif // FILTER_H
