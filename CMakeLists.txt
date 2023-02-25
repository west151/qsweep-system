cmake_minimum_required(VERSION 3.19)

project(qsweep-system VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_QSWEEP_SERVER "Enable building qsweep-server" ON)
option(BUILD_QSWEEP_CLIENT "Enable building qsweep-client" ON)
option(BUILD_QSWEEP_WRITER "Enable building qsweep-writer" ON)

if(BUILD_QSWEEP_SERVER)
    add_subdirectory(src/qsweep-server)
endif()

if(BUILD_QSWEEP_CLIENT)
    add_subdirectory(src/qsweep-client)
endif()

if(BUILD_QSWEEP_WRITER)
    add_subdirectory(src/qsweep-writer)
endif()
