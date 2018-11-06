SOURCES += \
    ../protocol/qsweepmessagelog.cpp \
    ../protocol/qsweeprequest.cpp \
    ../protocol/qsweeptopic.cpp \
    ../protocol/qsweepanswer.cpp \
    ../protocol/qsweepparams.cpp \
    ../protocol/qsweepspectr.cpp \
    ../protocol/qsweepsystemmonitor.cpp \
    $$PWD/src/protocol/sweep_message.cpp \
    $$PWD/src/protocol/sdr_info.cpp \
    $$PWD/src/protocol/data_spectr.cpp \
    $$PWD/src/protocol/data_log.cpp

HEADERS += \
    ../protocol/qsweepmessagelog.h \
    ../protocol/qsweeprequest.h \
    ../protocol/qsweeptopic.h \
    ../protocol/qsweepanswer.h \
    ../protocol/qsweepparams.h \
    ../protocol/qsweepspectr.h \
    ../protocol/qsweepsystemmonitor.h \
    ../protocol/constkeys.h \
    $$PWD/src/protocol/sweep_message.h \
    $$PWD/src/protocol/sdr_info.h \
    $$PWD/src/protocol/data_spectr.h \
    $$PWD/src/protocol/data_log.h


INCLUDEPATH += \
    $$PWD/src/protocol

DEPENDPATH
    $$PWD/src/protocol
