#include "MorphoDistance.h"

#include "BinaryMorphology.h"

const int MorphoDistance::SE_SQUARE_ARR[] = {1, 1, 1,
                                             1, 1, 1,
                                             1, 1, 1};

const int MorphoDistance::SE_CROSS_ARR[] = {0, 1, 0,
                                            1, 1, 1,
                                            0, 1, 0};

MorphoDistance::MorphoDistance(ImageProcessor* imageProcessor) :
    imageProcessor(imageProcessor),
    distanceImage(0),
    useSquareSe(true)
{
}

MorphoDistance::~MorphoDistance()
{
    if (distanceImage) {
        delete distanceImage;
    }
}

bool MorphoDistance::isUseSquareSe() const
{
    return useSquareSe;
}

QImage* MorphoDistance::getDistanceImage(const bool useSquareSe)
{
    this->useSquareSe = useSquareSe;
    StructElement* se;
    if (useSquareSe) {
        se = new StructElement(SE_RATIO, SE_RATIO, 1, 1, SE_SQUARE_ARR);
    } else {
        se = new StructElement(SE_RATIO, SE_RATIO, 1, 1, SE_CROSS_ARR);
    }

    BinaryMorphology morpho(imageProcessor);
    QImage* origin = morpho.getOperatedImage();
    int size = origin->width() * origin->height();

    int* stepArr = new int[size];
    for (int i = 0; i < size; ++i) {
        stepArr[i] = 0;
    }

    // set each border to be current step
    int step = 0;
    QImage* lastImage = origin;
    QImage* curImage;
    uchar fore = morpho.getForeground();
    uchar back = morpho.getBackground();
    while (true) {
        ++step;
        curImage = morpho.doErosion(*se);

        // mark border to be step
        const uchar* cBits = curImage->constBits();
        const uchar* lBits = lastImage->constBits();
        for (int i = 0; i < size; ++i) {
            if (*cBits == back && *lBits == fore) {
                // find border, mark with step
                stepArr[i] = step;
            }
            // to next pixel
            cBits += 4;
            lBits += 4;
        }

        bool isFore, isBack;
        morpho.isOneColor(isFore, isBack);
        if (isBack) {
            break;
        }
        lastImage = curImage;
    }

    // new distance image according to stepArr
    double ratio;
    if (step == 0) {
        ratio = 1.0;
    } else {
        ratio = 255.0 / step;
    }
    if (distanceImage) {
        delete distanceImage;
    }
    distanceImage = new QImage(origin->size(), origin->format());
    uchar* dBits = distanceImage->bits();
    for (int i = 0; i < size; ++i) {
        for (int rgb = 0; rgb < 3; ++rgb) {
            *(dBits + rgb) = stepArr[i] * ratio;
        }
        dBits += 4;
    }

    delete se;
    delete []stepArr;
    return distanceImage;
}
