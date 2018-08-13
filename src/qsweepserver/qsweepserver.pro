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
    hackrfinfo.cpp \
    settings/sweepserversettings.cpp \
    systemmonitorworker.cpp

HEADERS += \
    coresweep.h \
    sweepworker.h \
    hackrfinfo.h \
    constant.h \
    settings/sweepserversettings.h \
    systemmonitorworker.h
