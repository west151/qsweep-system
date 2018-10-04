SOURCES += \
    ../protocol/qsweepmessagelog.cpp \
    ../protocol/qsweeprequest.cpp \
    ../protocol/qsweeptopic.cpp \
    ../protocol/qsweepanswer.cpp \
    ../protocol/qhackrfinfo.cpp \
    ../protocol/qsweepparams.cpp \
    ../protocol/qsweepspectr.cpp \
    ../protocol/qsweepsystemmonitor.cpp

HEADERS += \
    ../protocol/qsweepmessagelog.h \
    ../protocol/qsweeprequest.h \
    ../protocol/qsweeptopic.h \
    ../protocol/qsweepanswer.h \
    ../protocol/qhackrfinfo.h \
    ../protocol/qsweepparams.h \
    ../protocol/qsweepspectr.h \
    ../protocol/qsweepsystemmonitor.h \
    ../protocol/constkeys.h


INCLUDEPATH += \
    $$PWD/src/protocol

DEPENDPATH
    $$PWD/src/protocol
