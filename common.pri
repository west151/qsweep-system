CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# windows
win32: OS_SUFFIX = win32
# linux
linux: OS_SUFFIX = linux

CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
    LIB_SUFFIX = d

    PROJECT_ROOT_PATH = $${PWD}

    BUILD_PATH = $${PROJECT_ROOT_PATH}/build/$${OS_SUFFIX}/$${BUILD_FLAG}
    BUILD_PATH_TARGET = $${BUILD_PATH}/$${TARGET}/
    BUILD_BIN_PATH = $${BUILD_PATH}/bin

    #DESTDIR = $${BUILD_BIN_PATH}

    RCC_DIR = $${BUILD_PATH_TARGET}/rcc/
    UI_DIR = $${BUILD_PATH_TARGET}/ui/
    MOC_DIR = $${BUILD_PATH_TARGET}/moc/
    OBJECTS_DIR = $${BUILD_PATH_TARGET}/obj/
} else {
    BUILD_FLAG = release
}

DESTDIR = $${PWD}/bin
