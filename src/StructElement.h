#ifndef STRUCTELEMENT_H
#define STRUCTELEMENT_H

class StructElement
{
public:
    enum SE_Type {
        ST_SQUARE,
        ST_CIRCLE,
        ST_CROSS,
        ST_DIAMOND,
        ST_CUSTOMED
    };

    StructElement(int width, int height, int originX, int originY,
                  const int* arr);
    StructElement(int radius, SE_Type seType,
                  const int* arr = 0);
    ~StructElement();

    // get min x according to origin, inclusively
    int getMinX() const;
    // get max x according to origin, exclusively
    int getMaxX() const;
    // get min y according to origin, inclusively
    int getMinY() const;
    // get max y according to origin, exclusively
    int getMaxY() const;

    // get value according to origin
    int getValue(int x, int y) const;
    // set value according to origin
    void setValue(int x, int y, int value);

    int getWidth() const;
    int getHeight() const;
    int getLength() const;

private:
    int width;
    int height;

    int length;
    int* arr;

    int originX;
    int originY;
};


#endif // STRUCTELEMENT_H
