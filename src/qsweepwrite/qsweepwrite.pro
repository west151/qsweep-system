QT -= gui
QT += mqtt sql

CONFIG += c++11 console
CONFIG -= app_bundle

include(../../common.pri)
include(../../protocol.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    qsweepwrite.cpp \
    core_sweep_write.cpp \
    sweep_write_settings.cpp \
    database/db_manager.cpp \
    provider/mqtt_provider.cpp \
    database/db_cleaner.cpp \
    database/db_path_monitor.cpp \
    database/db_system_worker.cpp \
    database/db_reader_worker.cpp \
    database/db_writer_worker.cpp \
    database/db_state_workers.cpp

# Default rules for deployment.
unix: target.path = /opt/qsweepwrite/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    core_sweep_write.h \
    sweep_write_settings.h \
    database/db_manager.h \
    provider/mqtt_provider.h \
    database/db_cleaner.h \
    database/db_path_monitor.h \
    database/db_const.h \
    database/db_system_worker.h \
    database/db_reader_worker.h \
    database/db_writer_worker.h \
    database/db_state_workers.h
