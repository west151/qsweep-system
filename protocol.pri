INCLUDEPATH += \
    $$PWD/lib/qsweepprotocol

DEPENDPATH
    $$PWD/lib/qsweepprotocol

CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
    LIB_SUFFIX = d
} else {
    BUILD_FLAG = release
}

LIBS += \
    -L$${PWD}/bin -lqsweepprotocol
