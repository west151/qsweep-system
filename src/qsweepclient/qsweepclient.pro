QT += quick mqtt

include(../../common.pri)
include(../../protocol.pri)

HEADERS += \
    coresweepclient.h \
    userinterface.h

SOURCES += \
    coresweepclient.cpp \
    userinterface.cpp \
    qsweepclient.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
