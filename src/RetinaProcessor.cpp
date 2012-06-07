#include "RetinaProcessor.h"

#ifdef DEBUG_TIME
#include <ctime>
#include <QDebug>
#endif

#include <math.h>

#include <QPainter>

#include "ImageProcessor.h"
#include "MedianFilter.h"
#include "SobelOperator.h"
#include "StructElement.h"

RetinaProcessor::RetinaProcessor(QImage* originImage, QImage* maskImage) :
    ImageProcessor(originImage),
    maskImage(maskImage),
    imageSize(originImage->width() * originImage->height()),
    retinaImage(0),
    centerImage(0),
    morpho(0)
{
    if (maskImage) {
        maskPtr = new bool[imageSize];
        const uchar* bits = maskImage->constBits();
        maskSize = 0;
        for (int i = 0; i < imageSize; ++i) {
            if (*bits == 0) {
                maskPtr[i] = false;
            } else {
                maskPtr[i] = true;
                ++maskSize;
            }
            bits += PIXEL_SIZE;
        }
    } else {
        maskPtr = 0;
    }
}

RetinaProcessor::~RetinaProcessor()
{
    if (maskPtr) {
        delete []maskPtr;
    }

    // retinaImage is delete within morpho
    if (morpho) {
        delete morpho;
    }

    if (centerImage) {
        delete centerImage;
    }
}

QImage* RetinaProcessor::getGrayScaleImage()
{
    if (grayScaleImage) {
        return grayScaleImage;
    }
    grayScaleImage = new QImage(originImage->size(), originImage->format());

    // pointer to originBits being processed
    const uchar* originPtr = originImage->constBits();
    // pointer to grayBits being processed
    uchar* grayPtr = grayScaleImage->bits();

    for (int i = 0; i < imageSize; ++i) {
        if (maskPtr[i]) {
            // calculate gray value according to strategy
            uchar grayValue = getGrayValue(originPtr, grayScalePolicy);
            for (int rgb = 0; rgb < 3; ++rgb) {
                *(grayPtr + rgb) = grayValue;
            }
        } else {
            for (int rgb = 0; rgb < 3; ++rgb) {
                *(grayPtr + rgb) = 0;
            }
        }
        // set alpha value to be 255
        *(grayPtr + 3) = MAX_OF_8BITS;
        // move pointer of origin and gray scale pointer to next pixel
        originPtr += PIXEL_SIZE;
        grayPtr += PIXEL_SIZE;
    }
    return grayScaleImage;
}

int* RetinaProcessor::getHistogram()
{
    // lazy calculation
    if (!isHisCaled) {
        // make sure gray scale image is calculated
        getGrayScaleImage();
        // init arrays
        for (int i = 0; i < RANGE_OF_8BITS; ++i) {
            histogram[i] = 0;
        }

        // pointer to pixels being processed which will not change
        // pixels in grayImage
        const uchar* grayPtr = grayScaleImage->constBits();

        int size = grayScaleImage->size().width() *
                grayScaleImage->size().height();
        for (int i = 0; i < size; ++i) {
            if (maskPtr[i]) {
                histogram[*grayPtr]++;
            }
            // point to next pixel
            grayPtr += PIXEL_SIZE;
        }

        // calculate accumulated histogram and weighted sum
        int sum = 0;
        weightedHisSum = 0;
        for (int i = 0; i < RANGE_OF_8BITS; ++i) {
            sum += histogram[i];
            accHistogram[i] = sum;
            weightedHisSum += i * histogram[i];
        }

        isHisCaled = true;
    }
    return histogram;
}

QImage* RetinaProcessor::getEqualImage()
{
    if (equalImage) {
        return equalImage;
    }
    getHistogram();
    getGrayScaleImage();

    // min level of rgb, within [0, 256)
    int minLevel;
    for (int i = 0; i < RANGE_OF_8BITS; ++i) {
        if (histogram[i] > 0) {
            minLevel = i;
            break;
        }
    }
    // max level of rgb, within [0, 256)
    int maxLevel;
    for (int i = RANGE_OF_8BITS - 1; i >= 0; --i) {
        if (histogram[i] > 0) {
            maxLevel = i;
            break;
        }
    }

    equalImage = new QImage(originImage->width(), originImage->height(),
                            originImage->format());
    uchar* bits = equalImage->bits();
    const uchar* oBits = grayScaleImage->constBits();

    int size = equalImage->width() * equalImage->height();
    for (int i = 0; i < size; ++i) {
        uchar value;
        if (maskPtr[i]) {
            value = (double)accHistogram[*oBits] / maskSize * MAX_OF_8BITS;
        } else {
            value = 0;
        }
        for (int rgb = 0; rgb < 3; ++rgb) {
            *bits = value;
            ++bits;
            ++oBits;
        }
        ++bits;
        ++oBits;
    }
    return equalImage;
}

QImage* RetinaProcessor::getRetinaImage()
{
#ifdef DEBUG_TIME
    time_t start;
    time(&start);
#endif

    if (morpho) {
        delete morpho;
    }

    // histogram equalization
    getEqualImage();

    // median filter of 3x3
    MedianFilter median(equalImage, 3, false);
    retinaImage = median.getFilteredImage();

    // sobel operator
    SobelOperator sobel(retinaImage, false);
    retinaImage = sobel.getFilteredImage();

    // binary reconstruction
    ImageProcessor imageProcessor(retinaImage);
    morpho = new BinaryMorphology(&imageProcessor);
    StructElement se1(3, StructElement::ST_CROSS);
    retinaImage = morpho->getReconstructImage(se1);

    // binary closing
    StructElement se2(7, StructElement::ST_CIRCLE);
    retinaImage = morpho->doClosing(se2);
    // binary opening
    StructElement se3(3, StructElement::ST_CROSS);
    retinaImage = morpho->doOpening(se3);

    // delete circle boundary
    ImageProcessor boundProcessor(maskImage);
    BinaryMorphology boundMorpho(&boundProcessor);
    StructElement se4(9, StructElement::ST_CIRCLE);
    QImage* boundImage = boundMorpho.doErosion(se4);
    // intersection
    const uchar* bBits = boundImage->constBits();
    uchar* rBits = retinaImage->bits();
    for (int i = 0; i < imageSize; ++i) {
        if (!*bBits) {
            for (int rgb = 0; rgb < 3; ++rgb) {
                *(rBits + rgb) = 0;
            }
        }
        bBits += 4;
        rBits += 4;
    }

#ifdef DEBUG_TIME
    time_t end;
    time(&end);
    qDebug() << "Time used:" << (end - start);
#endif

    return retinaImage;
}

// Author: Tianyang Lei
QImage* RetinaProcessor::getCenterImage()
{
    if (centerImage) {
        return centerImage;
    }
    centerImage = new QImage(*originImage);

    int maxavg = -1;
    int maxg[2];
    QRect rect = centerImage->rect();

    for (int i=0; i<rect.width()-5; i++)
    {
        for (int j=0; j<rect.height()-5; j++)
        {
            int avg = 0;
            for (int x=0; x<5; x++)
                for (int y=0; y<5; y++)
                {

                        QRgb p = centerImage->pixel(i+x, j+y);
                        avg += (qRed(p) + qGreen(p) + qBlue(p)) / 3;
                }
            if (avg > maxavg)
            {
                maxg[0] = i+5, maxg[1] = j+5;
                maxavg = avg;
            }

        }
    }
    QRgb s = centerImage->pixel(maxg[0],maxg[1]);

    const int SLICE = 25;
    QPoint ap[SLICE];
    for (int i=0; i<SLICE; i++)
    {
        for (int j=70; j>=0; j--)
        {
            QRgb p = centerImage->pixel(
                        maxg[0]+(j)*cos(double(i)/SLICE*2*3.14159),
                        maxg[1]+(j)*sin(double(i)/SLICE*2*3.14159));
            if (abs(qRed(p) - qRed(s)) < 95
                    && abs(qGreen(p) - qGreen(s)) < 95
                    && abs(qBlue(p) - qBlue(s)) < 95)
            {
                ap[i].setX(maxg[0]+(j)*cos(double(i)/SLICE*2*3.14159));
                ap[i].setY(maxg[1]+(j)*sin(double(i)/SLICE*2*3.14159));
                break;
            }
        }
    }
    QPainter p;
    p.begin(centerImage);
    p.setBrush(QBrush(QColor(0, 0, 0)));
    p.drawRect(0, 0, centerImage->width(), centerImage->height());
    p.setBrush(QBrush(QColor(255, 255, 255)));
    p.drawPolygon(ap, SLICE);
    p.end();

    return centerImage;
}
