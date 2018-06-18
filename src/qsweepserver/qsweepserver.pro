QT -= gui
QT += mqtt

CONFIG += console
CONFIG -= app_bundle

include(../../common.pri)
include(../../hackrf.pri)
include(../../fftw.pri)
include(../../protocol.pri)

SOURCES += \
    qsweepserver.cpp \
    coresweep.cpp \
    sweepworker.cpp \
    hackrfinfo.cpp

HEADERS += \
    coresweep.h \
    sweepworker.h \
    hackrfinfo.h
