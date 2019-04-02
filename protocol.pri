SOURCES += \
    $$PWD/src/protocol/sweep_message.cpp \
    $$PWD/src/protocol/sdr_info.cpp \
    $$PWD/src/protocol/data_spectr.cpp \
    $$PWD/src/protocol/data_log.cpp \
    $$PWD/src/protocol/system_monitor.cpp \
    $$PWD/src/protocol/params_spectr.cpp \
    $$PWD/src/protocol/sweep_topic.cpp \
    $$PWD/src/protocol/broker_ctrl.cpp

HEADERS += \
    $$PWD/src/protocol/constkeys.h \
    $$PWD/src/protocol/sweep_message.h \
    $$PWD/src/protocol/sdr_info.h \
    $$PWD/src/protocol/data_spectr.h \
    $$PWD/src/protocol/data_log.h \
    $$PWD/src/protocol/system_monitor.h \
    $$PWD/src/protocol/params_spectr.h \
    $$PWD/src/protocol/sweep_topic.h \
    $$PWD/src/protocol/broker_ctrl.h


INCLUDEPATH += \
    $$PWD/src/protocol

DEPENDPATH
    $$PWD/src/protocol
