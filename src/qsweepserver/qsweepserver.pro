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
unix: LIBS += -lfftw3f

# HACKRF
unix: LIBS += -lhackrf

# Default rules for deployment.
unix: target.path = /opt/qsweep/bin
!isEmpty(target.path): INSTALLS += target
