cmake_minimum_required(VERSION 3.19)

project(qsweep-system VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_QSWEEP_SERVER "Enable building qsweep-server" ON)

if(BUILD_QSWEEP_SERVER)
    add_subdirectory(src/qsweep-server)
endif()
