#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <QImage>

#include "ImagePolicy.h"

class Algebra
{
public:
    // scale maskeImage to size of baseImage and then do algebra operation
    static QImage* getAlgebraImage(QImage* baseImage, QImage* maskImage,
                                   ImagePolicy::AlgebraPolicy algebraPolicy,
                                   ImagePolicy::ScalePolicy scalePolicy);

private:
    Algebra();

    static QImage* scaledImage;
    static QImage* resultImage;

    static QImage* getAddImage(QImage* baseImage);
    static QImage* getMinusImage(QImage* baseImage);
    static QImage* getMultiplyImage(QImage* baseImage);
    static QImage* getDivideImage(QImage* baseImage);
};

#endif // ALGEBRA_H
