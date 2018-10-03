QT -= gui
QT += mqtt

CONFIG += console
CONFIG -= app_bundle

include(../../common.pri)
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

# FFT
unix:!macx: LIBS += -lfftw3f

# HACKRF
unix:!macx: LIBS += -lhackrf
#INCLUDEPATH += /usr/local/include/libhackrf
#DEPENDPATH += /usr/local/include/libhackrf

## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
