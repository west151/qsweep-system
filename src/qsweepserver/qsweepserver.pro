QT -= gui
QT += mqtt

CONFIG += console
CONFIG -= app_bundle

include(../../common.pri)
include(../../protocol.pri)

SOURCE_WINDOWS_LIB += ../../install/windows/libraries

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

# windows FFT
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../share/3dparty/fftw/3.3.7/.libs/ -llibfftw3f.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../share/3dparty/fftw/3.3.7/.libs/ -llibfftw3f.dll
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../share/3dparty/fftw/3.3.7/.libs/ -llibfftw3f.dlld

INCLUDEPATH += $$PWD/../share/3dparty/fftw/3.3.7/api
DEPENDPATH += $$PWD/../share/3dparty/fftw/3.3.7/api

# windows hackrf
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../share/3dparty/hackrf/2018.01.1/host/build/libhackrf/src/ -llibhackrf.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../share/3dparty/hackrf/2018.01.1/host/build/libhackrf/src/ -llibhackrf.dll
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../share/3dparty/hackrf/2018.01.1/host/build/libhackrf/src/ -llibhackrf.dlld

INCLUDEPATH += $$PWD/../share/3dparty/hackrf/2018.01.1/host/libhackrf/src
DEPENDPATH += $$PWD/../share/3dparty/hackrf/2018.01.1/host/libhackrf/src

# Default rules for deployment.
unix: target.path = /opt/qsweep/bin
!isEmpty(target.path): INSTALLS += target

# copies the given files to the destination directory
defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # replace slashes in source path for Windows
        win32:file ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

# libusb
copyToDestDir($${SOURCE_WINDOWS_LIB}/libusb-1.0.dll, $${DESTDIR}/)
# libhackrf
copyToDestDir($${SOURCE_WINDOWS_LIB}/libhackrf.dll, $${DESTDIR}/)
# libfftw3f-3
copyToDestDir($${SOURCE_WINDOWS_LIB}/libfftw3f-3.dll, $${DESTDIR}/)
