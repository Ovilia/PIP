#ifndef ALGEBRAICOPERATION_H
#define ALGEBRAICOPERATION_H

#include <QImage>

class AlgebraicOperation
{
public:
    QImage* getAddedImage(QImage* image1, QImage* image2);

private:
    // avoid init since this is used as static class
    AlgebraicOperation();
};

#endif // ALGEBRAICOPERATION_H
