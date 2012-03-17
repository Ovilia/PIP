#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T16:51:04
#
#-------------------------------------------------

QT       += core gui

TARGET = PIP_Qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ImageWidget.cpp \
    ImageProcessor.cpp \
    HistogramDialog.cpp

HEADERS  += mainwindow.h \
    ImageWidget.h \
    ImageProcessor.h \
    HistogramDialog.h \
    ImagePolicy.h

FORMS    += mainwindow.ui \
    ImageWidget.ui \
    HistogramDialog.ui

RESOURCES += \
    resource.qrc





