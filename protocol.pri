SOURCES += \
    ../protocol/qsweeprequest.cpp \
    ../protocol/qsweeptopic.cpp \
    ../protocol/qsweepanswer.cpp \
    ../protocol/qsweepparams.cpp \
    ../protocol/qsweepspectr.cpp \
    $$PWD/src/protocol/sweep_message.cpp \
    $$PWD/src/protocol/sdr_info.cpp \
    $$PWD/src/protocol/data_spectr.cpp \
    $$PWD/src/protocol/data_log.cpp \
    $$PWD/src/protocol/system_monitor.cpp

HEADERS += \
    ../protocol/qsweeprequest.h \
    ../protocol/qsweeptopic.h \
    ../protocol/qsweepanswer.h \
    ../protocol/qsweepparams.h \
    ../protocol/qsweepspectr.h \
    ../protocol/constkeys.h \
    $$PWD/src/protocol/sweep_message.h \
    $$PWD/src/protocol/sdr_info.h \
    $$PWD/src/protocol/data_spectr.h \
    $$PWD/src/protocol/data_log.h \
    $$PWD/src/protocol/system_monitor.h


INCLUDEPATH += \
    $$PWD/src/protocol

DEPENDPATH
    $$PWD/src/protocol
