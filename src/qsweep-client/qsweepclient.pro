QT += charts quick mqtt sql

include(../../common.pri)
include(../../protocol.pri)

HEADERS += \
    coresweepclient.h \
    model/hackrf_info_model.h \
    model/message_log_model.h \
    model/ranges_template_model.h \
    settings/client_settings.h \
    template/ranges_template.h \
    systemmonitorinterface.h \
    statesweepclient.h \
    chart/surface_spectr.h \
    chart/spectr_item.h \
    spectr/ta_spectr.h \
    database/db_local_state.h \
    model/params_spectr_model.h \
    user_interface.h

SOURCES += \
    coresweepclient.cpp \
    model/hackrf_info_model.cpp \
    model/message_log_model.cpp \
    model/ranges_template_model.cpp \
    settings/client_settings.cpp \
    template/ranges_template.cpp \
    qsweepclient.cpp \
    systemmonitorinterface.cpp \
    statesweepclient.cpp \
    chart/surface_spectr.cpp \
    chart/spectr_item.cpp \
    spectr/ta_spectr.cpp \
    database/db_local_state.cpp \
    model/params_spectr_model.cpp \
    user_interface.cpp

RESOURCES += \
    $$files(qml/*.qml) \
    qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
unix: target.path = /opt/qsweep/bin
!isEmpty(target.path): INSTALLS += target
