#include <QBrush>
#include <QPainter>
#include <Qpen>
#include "HistogramPlot.h"

#include <stdio.h>

HistogramPlot::HistogramPlot(int* dataset) :
    hasSetRgb(false),
    lowerThreshold(0),
    higherThreshold(0),
    isGrayVisible(true),
    borderMargin(5),
    borderWidth(1),
    chartMargin(50),
    plotHeight(400),
    barGap(0),
    barWidth(2),
    rgbLineWidth(1)
{
    // set rgb invisible
    for (int i = 0; i < 3; ++i) {
        isRgbVisible[i] = false;
    }

    plotWidth = RANGE_OF_8BITS * (barGap + barWidth) - barGap;
    // make sure min size is enough for display
    setMinimumSize(plotWidth + 2 * (chartMargin + borderMargin),
                   plotHeight + 2 * (chartMargin + borderMargin));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // copy dataset
    for (int i = 0; i < RANGE_OF_8BITS; ++i) {
        grayDataset[i] = *(dataset + i);
        if (grayDataset[i] > maxData[0]) {
            maxData[0] = grayDataset[i];
        }
    }

    // calculate bar height according to the max height
    for (int i = 0; i < RANGE_OF_8BITS; ++i) {
        if (maxData[0] == 0) {
            barHeight[i][0] = 0;
        } else {
            barHeight[i][0] = grayDataset[i] * plotHeight / maxData[0];
        }
    }
}

HistogramPlot::~HistogramPlot()
{
}

void HistogramPlot::addRgbDataset(int* dataset)
{
    int* ptr = dataset;
    for (int i = 0; i < RANGE_OF_8BITS; ++i) {
        for (int j = 0; j < 3; ++j) {
            rgbDataset[i][j] = *ptr;
            if (rgbDataset[i][j] > maxData[j + 1]) {
                maxData[j + 1] = rgbDataset[i][j];
            }
            ++ptr;
        }
    }

    // calculate bar height according to the max height
    for (int i = 0; i < RANGE_OF_8BITS; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (maxData[j + 1] == 0) {
                barHeight[i][j + 1] = 0;
            } else {
                barHeight[i][j + 1] =
                        rgbDataset[i][j] * plotHeight / maxData[j + 1];
            }
        }
    }

    hasSetRgb = true;
}

void HistogramPlot::setThresholdValue(int lower, int higher)
{
    lowerThreshold = lower;
    higherThreshold = higher;
}

void HistogramPlot::setGrayVisible(bool isVisible)
{
    isGrayVisible = isVisible;
}

void HistogramPlot::setRedVisible(bool isVisible)
{
    isRgbVisible[0] = isVisible;
}

void HistogramPlot::setGreenVisible(bool isVisible)
{
    isRgbVisible[1] = isVisible;
}

void HistogramPlot::setBlueVisible(bool isVisible)
{
    isRgbVisible[2] = isVisible;
}

void HistogramPlot::paintEvent(QPaintEvent *)
{
    int width = this->size().width();
    int height = this->size().height();
    QPainter* paint=new QPainter;
    paint->begin(this);
    // draw border
    paint->setPen(QPen(Qt::gray, borderWidth));
    paint->drawRect(borderMargin, borderMargin,
                    width - 2 * borderMargin, height - 2 * borderMargin);

    int plotLeft = (width - plotWidth) / 2;
    int plotBottom = (height + plotHeight) / 2;
    // gray
    if (isGrayVisible) {
        int left = plotLeft;
        int bottom = plotBottom;
        for (int i = 0; i < RANGE_OF_8BITS; ++i) {
            paint->setPen(QPen(QColor(i, i, i)));
            paint->setBrush(QBrush(QColor(i, i, i)));
            paint->drawRect(left, bottom - barHeight[i][0],
                            barWidth, barHeight[i][0]);
            left += barWidth + barGap;
        }
    }

    // rgb
    if (hasSetRgb) {
        for (int rgb = 0; rgb < 3; ++rgb) {
            if (isRgbVisible[rgb]) {
                switch (rgb) {
                case 0:
                    paint->setPen(QPen(Qt::red, rgbLineWidth));
                    break;
                case 1:
                    paint->setPen(QPen(Qt::green, rgbLineWidth));
                    break;
                case 2:
                    paint->setPen(QPen(Qt::blue, rgbLineWidth));
                    break;
                }
                int lastX = plotLeft;
                int lastY = plotBottom - barHeight[0][rgb + 1];
                for (int i = 1; i < RANGE_OF_8BITS; ++i)
                {
                    int curX = lastX + barWidth + barGap;
                    int curY = plotBottom - barHeight[i][rgb + 1];
                    paint->drawLine(lastX, lastY, curX, curY);
                    lastX = curX;
                    lastY = curY;
                }
            }
        }
    }

    paint->end();
}
