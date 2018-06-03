QT -= gui
QT += mqtt

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/coresweep.cpp \
    src/sweepworker.cpp \
    src/hackrfinfo.cpp \
    src/ctrlsweepworker.cpp \
    src/sweepparams.cpp

HEADERS += \
    src/coresweep.h \
    src/sweepworker.h \
    src/hackrfinfo.h \
    src/ctrlsweepworker.h \
    src/sweepparams.h

# HACKRF
unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lhackrf

INCLUDEPATH += $$PWD/../../../../usr/local/include/libhackrf
DEPENDPATH += $$PWD/../../../../usr/local/include/libhackrf

# FFT
unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lfftw3f

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

# MQTT
unix:!macx: LIBS += -L$$PWD~/work/git/new_qtmqtt/lib/ -lQt5Mqtt

INCLUDEPATH += $$PWD~/work/git/new_qtmqtt/include
DEPENDPATH += $$PWD~/work/git/new_qtmqtt/include
