#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T16:51:04
#
#-------------------------------------------------

QT       += core gui

TARGET = PIP_Qt
TEMPLATE = app


SOURCES += main.cpp\
    ImageWidget.cpp \
    ImageProcessor.cpp \
    HistogramDialog.cpp \
    HistogramPlot.cpp \
    MainWindow.cpp \
    Filter.cpp \
    LinearFilter.cpp \
    SobelOperator.cpp \
    Operator.cpp \
    RobertsOperator.cpp \
    FilterDialog.cpp \
    PrewittOperator.cpp \
    GaussianFilter.cpp \
    MeanFilter.cpp \
    MedianFilter.cpp \
    ImageLabel.cpp \
    AlgebraicOperation.cpp \
    ImageScaler.cpp \
    ScaleDialog.cpp

HEADERS  += \
    ImageWidget.h \
    ImageProcessor.h \
    HistogramDialog.h \
    ImagePolicy.h \
    HistogramPlot.h \
    MainWindow.h \
    Filter.h \
    LinearFilter.h \
    SobelOperator.h \
    Operator.h \
    RobertsOperator.h \
    FilterDialog.h \
    PrewittOperator.h \
    GaussianFilter.h \
    MeanFilter.h \
    MedianFilter.h \
    ImageLabel.h \
    AlgebraicOperation.h \
    ImageScaler.h \
    ScaleDialog.h

FORMS    += \
    ImageWidget.ui \
    HistogramDialog.ui \
    MainWindow.ui \
    FilterDialog.ui \
    ScaleDialog.ui

RESOURCES += \
    resource.qrc







