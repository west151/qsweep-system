#-------------------------------------------------
#
# Project created by QtCreator 2018-06-08T13:43:46
#
#-------------------------------------------------

QT       -= gui

TARGET = qsweepprotocol
TEMPLATE = lib

DEFINES += QSWEEPPROTOCOL_LIBRARY

include(../../common.pri)

SOURCES += \
    qsweepmessagelog.cpp \
    qsweeprequest.cpp \
    qsweeptopic.cpp \
    qsweepanswer.cpp \
    qhackrfinfo.cpp \
    qsweepparams.cpp \
    qsweepspectr.cpp \
    qsweepsystemmonitor.cpp

HEADERS += \
    qsweepprotocol_global.h \
    qsweepmessagelog.h \
    qsweeprequest.h \
    qsweeptopic.h \
    qsweepanswer.h \
    qhackrfinfo.h \
    qsweepparams.h \
    qsweepspectr.h \
    qsweepsystemmonitor.h \
    constkeys.h
