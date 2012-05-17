#include "StructElement.h"

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
