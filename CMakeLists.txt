cmake_minimum_required(VERSION 3.19)

project(qsweep-system VERSION 1.0.0 LANGUAGES CXX)

# qsweep-server
option(BUILD_QSWEEP_SERVER "Enable building qsweep-server" ON)
option(MAKE_DEB_QSWEEP_SERVER "Enable make deb qsweep-server" ON)
# qsweep-client
option(BUILD_QSWEEP_CLIENT "Enable building qsweep-client" ON)
# qsweep-writer
option(BUILD_QSWEEP_WRITER "Enable building qsweep-writer" ON)
# qsweep-remote-control
option(BUILD_QSWEEP_REMOTE_CONTROL "Enable building qsweep-remote-control" ON)

include (cmake/output.cmake)

if(BUILD_QSWEEP_SERVER)
    add_subdirectory(src/qsweep-server)
endif()

if(BUILD_QSWEEP_CLIENT)
    add_subdirectory(src/qsweep-client)
endif()

if(BUILD_QSWEEP_WRITER)
    add_subdirectory(src/qsweep-writer)
endif()

if(BUILD_QSWEEP_REMOTE_CONTROL)
    add_subdirectory(src/qsweep-remote-control)
endif()
