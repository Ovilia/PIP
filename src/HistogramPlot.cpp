#include <QBrush>
#include <QPainter>
#include <Qpen>
#include "HistogramPlot.h"

HistogramPlot::HistogramPlot(int* dataset) :
    hasSetRgb(false),
    lowerThreshold(0),
    higherThreshold(0),
    isGrayVisible(true),
    isRgbVisible(false),
    borderMargin(5),
    borderWidth(1),
    chartMargin(50),
    plotHeight(400),
    barGap(0),
    barWidth(2)
{
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
        barHeight[i][0] = grayDataset[i] * plotHeight / maxData[0];
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
            barHeight[i][j + 1] =
                    rgbDataset[i][j] * plotHeight / maxData[j + 1];
        }
    }

    hasSetRgb = true;
}

void HistogramPlot::setThresholdValue(int lower, int higher)
{
    lowerThreshold = lower;
    higherThreshold = higher;
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

    // gray
    if (isGrayVisible) {
        paint->setPen(QPen(Qt::gray, 0));
        paint->setBrush(QBrush(Qt::gray, Qt::SolidPattern));
        int left = (width - plotWidth) / 2;
        int bottom = (height + plotHeight) / 2;
        for (int i = 0; i < RANGE_OF_8BITS; ++i) {
            paint->drawRect(left, bottom - barHeight[i][0],
                            barWidth, barHeight[i][0]);
            left += barWidth + barGap;
        }
    }

    paint->end();
}
