#ifndef IMAGESCALER_H
#define IMAGESCALER_H

#include <QImage>

#include "ImagePolicy.h"

class ImageScaler
{
public:
    // the outside is responsible for destruct
    static QImage* getScaledImage(const QImage* originImage,
                                  int newWidth, int newHeight,
                                  ImagePolicy::ScalePolicy policy);

private:
    ImageScaler();

    static QImage* getNearestImage(const QImage* originImage,
                                   int newWidth, int newHeight);
    static QImage* getBilinearImage(const QImage* originImage,
                                    int newWidth, int newHeight);
};

#endif // IMAGESCALER_H
