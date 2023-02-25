cmake_minimum_required(VERSION 3.19)

project(qsweep-server
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "qsweep-protocol"
    HOMEPAGE_URL https://github.com/west151/qsweep-system
)

file(GLOB_RECURSE HEADER
    *.h
)
file(GLOB_RECURSE SOURCES
    *.cpp
)

