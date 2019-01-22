QT += charts quick mqtt sql

include(../../common.pri)
include(../../protocol.pri)

HEADERS += \
    coresweepclient.h \
    userinterface.h \
    model/messagelogmodel.h \
    model/hackrfinfomodel.h \
    settings/sweepclientsettings.h \
    systemmonitorinterface.h \
    statesweepclient.h \
    chart/surface_spectr.h \
    chart/spectr_item.h \
    spectr/ta_spectr.h \
    database/db_local_state.h \
    model/params_spectr_model.h

SOURCES += \
    coresweepclient.cpp \
    userinterface.cpp \
    qsweepclient.cpp \
    model/messagelogmodel.cpp \
    model/hackrfinfomodel.cpp \
    settings/sweepclientsettings.cpp \
    systemmonitorinterface.cpp \
    statesweepclient.cpp \
    chart/surface_spectr.cpp \
    chart/spectr_item.cpp \
    spectr/ta_spectr.cpp \
    database/db_local_state.cpp \
    model/params_spectr_model.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
unix: target.path = /opt/qsweep/bin
!isEmpty(target.path): INSTALLS += target
