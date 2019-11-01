QT += quick mqtt sql

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

HEADERS += \
    core_sweep_remote_control.h \
    manager/net_manager.h \
    model/fft_width.h \
    model/fft_width_model.h \
    model/lna_gain.h \
    model/lna_gain_model.h \
    model/top_model.h \
    model/ui_model.h \
    model/vga_gain.h \
    model/vga_gain_model.h \
    provider/mqtt_provider.h \
    settings/mqtt_provider_settings.h \
    settings/remote_control_settings.h

SOURCES += \
    core_sweep_remote_control.cpp \
    manager/net_manager.cpp \
    model/fft_width.cpp \
    model/fft_width_model.cpp \
    model/lna_gain.cpp \
    model/lna_gain_model.cpp \
    model/top_model.cpp \
    model/ui_model.cpp \
    model/vga_gain.cpp \
    model/vga_gain_model.cpp \
    provider/mqtt_provider.cpp \
    qsweepremotecontrol.cpp \
    settings/mqtt_provider_settings.cpp \
    settings/remote_control_settings.cpp

RESOURCES += \
#    $$files(qml/*.qml) \
    qml.qrc

# Default rules for deployment.
unix: target.path = /opt/qsweepremotecontrol/bin
!isEmpty(target.path): INSTALLS += target
