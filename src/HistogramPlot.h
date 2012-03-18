#ifndef HISTOGRAMPLOT_H
#define HISTOGRAMPLOT_H

#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QWidget>

class HistogramPlot : public QWidget
{
public:
    static const int RANGE_OF_8BITS = 256;

    HistogramPlot(int* dataset);
    ~HistogramPlot();

    void addRgbDataset(int* dataset);
    void setThresholdValue(int lower, int higher);

    void setGrayVisible(bool isVisible);
    void setRedVisible(bool isVisible);
    void setGreenVisible(bool isVisible);
    void setBlueVisible(bool isVisible);

    void paintEvent(QPaintEvent *);

private:
    int grayDataset[RANGE_OF_8BITS];
    int rgbDataset[RANGE_OF_8BITS][3];
    // if rgb dataset is added
    bool hasSetRgb;

    // max value of gray and rgb
    int maxData[4];

    int lowerThreshold;
    int higherThreshold;

    bool isGrayVisible;
    bool isRgbVisible[3];

    // margin from the most outside border to chart
    int borderMargin;
    int borderWidth;
    // margin from chart to plot
    int chartMargin;
    // width and height for bars
    int plotWidth;
    int plotHeight;
    // width between bars
    int barGap;
    // width of bars
    int barWidth;
    // height of bars, gray and rgb
    int barHeight[RANGE_OF_8BITS][4];
    // line width of rgb
    int rgbLineWidth;
};

#endif // HISTOGRAMPLOT_H
