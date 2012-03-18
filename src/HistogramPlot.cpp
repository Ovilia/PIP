#include <QBrush>
#include <QPainter>
#include <Qpen>
#include "HistogramPlot.h"

HistogramPlot::HistogramPlot(int* dataset) :
    hasSetRgb(false),
    lowerThreshold(0),
    higherThreshold(0),
    isGrayVisible(true),
    borderMargin(5),
    borderWidth(1),
    chartMargin(60),
    plotHeight(400),
    barGap(0),
    barWidth(2),
    rgbLineWidth(1),
    axisMargin(10),
    axisWidth(1),
    stickLength(5),
    xLabelMargin(15),
    yLabelMargin(30),
    xLabelAmt(9),
    yLabelAmt(10),
    titleMargin(50),
    titleWidth(200),
    titleHeight(50)
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

    setGrayDataset(dataset);
}

HistogramPlot::~HistogramPlot()
{
}

void HistogramPlot::setGrayDataset(int* dataset)
{
    // copy dataset
    maxData[0] = 0;
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

void HistogramPlot::setRgbDataset(int* dataset)
{
    for (int i = 1; i < 4; ++i) {
        maxData[i] = 0;
    }
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
    int plotTop = plotBottom - plotHeight;
    int plotRight = plotLeft + plotWidth;

    // gray
    if (isGrayVisible) {
        int left = plotLeft;
        int bottom = plotBottom;
        paint->setPen(QPen(Qt::black));
        for (int i = 0; i < RANGE_OF_8BITS; ++i) {
            paint->setBrush(QBrush(QColor(i, i, i)));
            paint->drawRect(left, bottom - barHeight[i][0],
                            barWidth, barHeight[i][0]);
            left += barWidth + barGap;
        }
    }

    // threshold area
    paint->setPen(QPen(QColor(255, 255, 0, 64)));
    paint->setBrush(QBrush(QColor(255, 255, 0, 64)));
    paint->drawRect(plotLeft + lowerThreshold * (barWidth + barGap),
                    plotTop,
                    (higherThreshold - lowerThreshold) * (barWidth + barGap),
                    plotHeight);

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

    // x-axis
    int xHeight = plotBottom + axisMargin;
    // main axis
    paint->setPen(QPen(Qt::black, axisWidth));
    paint->drawLine(plotLeft, xHeight, plotRight, xHeight);
    // labels
    int left = plotLeft;
    int gap = plotWidth / (xLabelAmt - 1);
    for (int i = 0; i < xLabelAmt; ++i) {
        int value = RANGE_OF_8BITS * i / (xLabelAmt - 1);
        if (value == RANGE_OF_8BITS) {
            value--;
        }
        paint->drawText(left, xHeight + xLabelMargin,
                        QString::number(value));
        left += gap;
    }

    // y-axis
    int yLeft = plotLeft - axisMargin;
    // main axis
    paint->drawLine(yLeft, plotTop,
                    yLeft, plotBottom);
    // labels
    int top = plotTop;
    gap = plotHeight / (yLabelAmt - 1);
    for (int i = 0; i < yLabelAmt; ++i) {
        int value = maxData[0] * (yLabelAmt - 1 - i) / (yLabelAmt - 1);
        paint->drawText(yLeft - yLabelMargin, top,
                        QString::number(value));
        top += gap;
    }

    // title
    int titleLeft = plotLeft + (plotWidth - titleWidth) / 2;
    int titleTop = plotTop - titleMargin;
    paint->setFont(QFont("Times", 30));
    paint->drawText(titleLeft, titleTop, titleWidth, titleHeight,
                    Qt::AlignCenter, tr("Histogram"));

    paint->end();
}
