QT += charts quick mqtt

include(../../common.pri)
include(../../protocol.pri)

HEADERS += \
    coresweepclient.h \
    userinterface.h \
    model/messagelogmodel.h \
    model/hackrfinfomodel.h \
    settings/sweepclientsettings.h \
    chart/datasource.h \
    chart/waterfallitem.h \
    systemmonitorinterface.h \
    statesweepclient.h \
    chart/gl_plots.h \
    chart/spectr_item.h \
    chart/surface_spectr.h

SOURCES += \
    coresweepclient.cpp \
    userinterface.cpp \
    qsweepclient.cpp \
    model/messagelogmodel.cpp \
    model/hackrfinfomodel.cpp \
    settings/sweepclientsettings.cpp \
    chart/datasource.cpp \
    chart/waterfallitem.cpp \
    systemmonitorinterface.cpp \
    statesweepclient.cpp \
    chart/gl_plots.cpp \
    chart/spectr_item.cpp \
    chart/surface_spectr.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
unix: target.path = /opt/qsweep/bin
!isEmpty(target.path): INSTALLS += target
