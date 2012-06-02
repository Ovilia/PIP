#include "StructElement.h"

#include <qmath.h>

StructElement::StructElement(int width, int height, int originX, int originY,
                             const int* arr) :
    width(width),
    height(height),
    length(width * height),
    originX(originX),
    originY(originY)
{
    this->arr = new int[length];
    for (int i = 0; i < length; ++i) {
        this->arr[i] = arr[i];
    }
}

StructElement::StructElement(int radius, SE_Type seType, const int* arr) :
    width(radius),
    height(radius),
    length(width * height),
    originX(radius / 2),
    originY(radius / 2)
{
    this->arr = new int[length];
    switch (seType) {
    case ST_SQUARE:
        // all 1
        for (int i = 0; i < length; ++i) {
            this->arr[i] = 1;
        }
        break;

    case ST_CIRCLE:
        for (int w = 0; w < radius; ++w) {
            for (int h = 0; h < radius; ++h) {
                if ((w - radius / 2) * (w - radius / 2) +
                        (h - radius / 2) * (h - radius / 2)
                        < length / 4) {
                    // within a circle
                    this->arr[w + h * radius] = 1;
                } else {
                    this->arr[w + h * radius] = 0;
                }
            }
        }
        break;

    case ST_CROSS:
    {
        for (int i = 0; i < length; ++i) {
            this->arr[i] = 0;
        }
        int center = radius / 2;
        for (int i = 0; i < radius; ++i) {
            this->arr[center * radius + i] = 1;
            this->arr[center + i * radius] = 1;
        }
    }
        break;

    case ST_DIAMOND:
    {
        int center = radius / 2;
        for (int i = 0; i < radius; ++i) {
            int start = qAbs(i - center);
            int end = center * 2 - start;
            for (int j = start; j <= end; ++j) {
                this->arr[i * radius + j] = 1;
            }
        }
    }
        break;

    case ST_CUSTOMED:
    {
        for (int i = 0; i < length; ++i) {
            this->arr[i] = arr[i];
        }
        break;
    }
    }
}

StructElement::~StructElement()
{
    delete []arr;
}

int StructElement::getWidth() const
{
    return width;
}

int StructElement::getHeight() const
{
    return height;
}

int StructElement::getLength() const
{
    return length;
}

int StructElement::getMinX() const
{
    return -originX;
}

int StructElement::getMaxX() const
{
    return width - originX;
}

int StructElement::getMinY() const
{
    return -originY;
}

int StructElement::getMaxY() const
{
    return height - originY;
}

int StructElement::getValue(int x, int y) const
{
    return arr[(x + originX) + (y + originY) * width];
}

void StructElement::setValue(int x, int y, int value)
{
    arr[(x + originX) + (y + originY) * width] = value;
}
