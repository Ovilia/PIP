#ifndef IMAGESCALER_H
#define IMAGESCALER_H

#include <QImage>

#include "ImagePolicy.h"

class ImageScaler
{
public:
    // the outside is responsible for destruct
    static QImage* getScaledImage(QImage* originImage,
                                  int newWidth, int newHeight,
                                  ImagePolicy::ScalePolicy policy);

private:
    ImageScaler();

    static QImage* getNearestImage(QImage* originImage,
                                   int newWidth, int newHeight);
    static QImage* getBilinearImage(QImage* originImage,
                                    int newWidth, int newHeight);
};

#endif // IMAGESCALER_H
