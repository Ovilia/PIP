#include "Filter.h"

Filter::Filter(QImage* image, int* kernelPtr, int kernelRadio) :
    originImage(image),
    kernelIntPtr(kernelPtr),
    kernelDoublePtr(0),
    kernelRadio(kernelRadio)
{
}

Filter::Filter(
        QImage* image, double* kernelPtr, int kernelRadio) :
    originImage(image),
    kernelIntPtr(0),
    kernelDoublePtr(kernelPtr),
    kernelRadio(kernelRadio)
{
}

Filter::~Filter()
{
}

inline int Filter::get2DIndex(int columns, int x, int y)
{
    return columns * y + x;
}
