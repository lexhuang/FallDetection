    #-------------------------------------------------
#
# Project created by QtCreator 2015-03-14T06:09:56
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = JorjinTOF
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
#    TOFLib/src/capture.cpp \
    TOFLib/src/Device.cpp \
    TOFLib/src/regProg.cpp \
    TOFLib/src/Globals.cpp \
    TOFLib/src/voxelxu.cpp \
    TOFLib/src/tofmanager.cpp \
    TOFLib/src/voxelfw.cpp \
    capture.cpp

HEADERS += \
#       TOFLib/include/capture.h \
    TOFLib/include/Device.h \
    TOFLib/include/regProg.h \
    TOFLib/include/Globals.h \
    TOFLib/include/voxelxuLinux.h \
    TOFLib/include/OPT9220_regmap.h \
    TOFLib/include/tofmanager.h \
    TOFLib/include/voxelfw-linux.h \
    capture.h

INCLUDEPATH +=  /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2 \
                TOFLib/include

LIBS += /usr/local/lib/libopencv_highgui.so.2.4.9 \
        /usr/local/lib/libopencv_core.so.2.4.9    \
        /usr/local/lib/libopencv_imgproc.so.2.4.9
