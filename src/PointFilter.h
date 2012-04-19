#ifndef COMICFILTER_H
#define COMICFILTER_H

#include <QImage>

class PointFilter
{
public:
    PointFilter(QImage* image);
    ~PointFilter();

    QImage* getComicImage();
    QImage* getSepiaImage();

private:
    QImage* originImage;
    QImage* comicImage;
    QImage* sepiaImage;

    inline uchar getPixel(double value);
};

#endif // COMICFILTER_H
