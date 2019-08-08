QT -= gui
QT += mqtt

CONFIG += console
CONFIG -= app_bundle

include(../../common.pri)
include(../../protocol.pri)

SOURCES += \
    core_sweep.cpp \
    hackrf_info.cpp \
    parser_worker.cpp \
    process_worker.cpp \
    qsweepserver.cpp \
    settings/server_settings.cpp \
    spectrum_native_worker.cpp \
    systemmonitorworker.cpp

HEADERS += \
    core_sweep.h \
    hackrf_info.h \
    parser_worker.h \
    process_worker.h \
    settings/server_settings.h \
    constant.h \
    spectrum_native_worker.h \
    systemmonitorworker.h

# FFT
unix: LIBS += -lfftw3f

# HACKRF
unix: LIBS += -lhackrf

# Default rules for deployment.
unix: target.path = /opt/qsweep/bin
!isEmpty(target.path): INSTALLS += target
