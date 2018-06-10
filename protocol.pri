# windows
win32: OS_SUFFIX = win32
# linux
linux: OS_SUFFIX = linux

CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
    LIB_SUFFIX = d
} else {
    BUILD_FLAG = release
}

INCLUDEPATH += \
    $$PWD/lib/qsweepprotocol

DEPENDPATH
    $$PWD/lib/qsweepprotocol

LIBS += \
    -L$${PWD}/build/$${OS_SUFFIX}/$${BUILD_FLAG}/bin -lqsweepprotocol
