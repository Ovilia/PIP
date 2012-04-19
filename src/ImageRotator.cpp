#include "ImageRotator.h"

#include <qmath.h>

#define PI 3.1415926535
#define RADIAN(angle) ((angle)*PI/180.0)

ImageRotator::ImageRotator()
{
}

QImage* ImageRotator::getRotatedImage(QImage *old, int angle)
{
    int width = old->width();
    int height = old->height();

    double ltx = -(double)width / 2.0;
    double lty = (double)height / 2.0;
    double rtx = (double)width / 2.0;
    double rty = (double)height / 2.0;
    double lbx = -(double)width / 2.0;
    double lby = -(double)height / 2.0;
    double rbx = (double)width / 2.0;
    double rby = -(double)height / 2.0;

    double sina = sin(RADIAN(angle));
    double cosa = cos(RADIAN(angle));

    double ltnx = ltx * cosa + lty * sina;
    double ltny = -ltx * sina + lty * cosa;
    double rtnx = rtx * cosa + rty * sina;
    double rtny = -rtx * sina + rty * cosa;
    double lbnx = lbx * cosa + lby * sina;
    double lbny = -lbx * sina + lby * cosa;
    double rbnx = rbx * cosa + rby * sina;
    double rbny = -rbx * sina + rby * cosa;

    int newWidth = qMax(qAbs(rbnx - ltnx), qAbs(rtnx - lbnx));
    int newHeight = qMax(qAbs(rbny - ltny), qAbs(rtny - lbny));

    QImage* newimg = new QImage(newWidth, newHeight, old->format());
    uchar* bits = newimg->bits();
    const uchar* oBits = old->constBits();

    for (int j = 0; j < newHeight; j++) {
        for (int i = 0; i < newWidth; i++) {
            int x = (i - newWidth / 2) * cos(RADIAN(360 - angle))
                    + (-j + newHeight / 2) * sin(RADIAN(360 - angle));
            int y = -(i - newWidth / 2) * sin(RADIAN(360 - angle))
                    + (-j + newHeight / 2) * cos(RADIAN(360 - angle));
            if (x > width / 2 || x < -width / 2 ||
                    y > height / 2 || y < -height / 2) {
                for (int rgb = 0; rgb < 3; ++rgb) {
                    *(bits + 4 * (j * newWidth + i) + rgb) = 255;
                }
            } else {
                int newX = x + width / 2;
                int newY = qAbs(y - height / 2);
                for (int rgb = 0; rgb < 3; ++rgb) {
                    *(bits + 4 * (j * newWidth + i) + rgb) =
                            *(oBits + (newY * width + newX) * 4 + rgb);
                }
            }
        }
    }

    return newimg;
}
