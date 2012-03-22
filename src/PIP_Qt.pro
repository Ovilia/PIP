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
    PrewittOperator.cpp

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
    PrewittOperator.h

FORMS    += \
    ImageWidget.ui \
    HistogramDialog.ui \
    MainWindow.ui \
    FilterDialog.ui

RESOURCES += \
    resource.qrc































