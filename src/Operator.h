#ifndef OPERATOR_H
#define OPERATOR_H

#include <QImage>

#include "Filter.h"
#include "ImagePolicy.h"
#include "LinearFilter.h"

class LinearFilter;

class Operator : Filter
{
public:
    Operator(QImage* image, const int kernelRadio,
             const int* xKernel, const int* yKernel,
             const bool isColored, ImagePolicy::BorderPolicy policy);
    ~Operator();

    // overload function
    QImage* getFilteredImage();

    void setXEnabled(bool isEnabled);
    void setYEnabled(bool isEnabled);
    bool getXEnabled();
    bool getYEnabled();

protected:
    LinearFilter* xFilter;
    LinearFilter* yFilter;

    int kernelRadio;
    const int* xKernel;
    const int* yKernel;

    bool isXEnabled;
    bool isYEnabled;

    QImage* xOperatedImage;
    QImage* yOperatedImage;
    QImage* xyOperatedImage;

    // override function used for xyOperatedImage
    virtual int doFiltering(int x, int y, ColorOffset offset);
};

#endif // OPERATOR_H
