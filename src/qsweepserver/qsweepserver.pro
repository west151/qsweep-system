QT -= gui
QT += mqtt

CONFIG += console
CONFIG -= app_bundle

include(../../common.pri)
include(../../protocol.pri)

SOURCES += \
    core_sweep.cpp \
    hackrf_info.cpp \
    worker/parser_worker.cpp \
    qsweepserver.cpp \
    settings/server_settings.cpp \
    worker/spectrum_native_worker.cpp \
    worker/spectrum_process_worker.cpp \
    systemmonitorworker.cpp \
    worker/state_worker.cpp

HEADERS += \
    core_sweep.h \
    hackrf_info.h \
    worker/parser_worker.h \
    settings/server_settings.h \
    constant.h \
    worker/spectrum_native_worker.h \
    worker/spectrum_process_worker.h \
    systemmonitorworker.h \
    worker/state_worker.h

# FFT
unix: LIBS += -lfftw3f

# HACKRF
unix: LIBS += -lhackrf

# Default rules for deployment.
unix: target.path = /opt/qsweep/bin
!isEmpty(target.path): INSTALLS += target
