cmake_minimum_required(VERSION 3.19)

project(qsweep VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_QSWEEPSERVER "Enable building qsweepserver" ON)

if(BUILD_QSWEEPSERVER)
    add_subdirectory(src/qsweepserver)
endif()
