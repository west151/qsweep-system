cmake_minimum_required(VERSION 3.14)

project(qsweep-protocol LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(qsweep-protocol SHARED
  qsweep-protocol_global.h
  sweep_topic.cpp
  sweep_topic.h
)

target_link_libraries(qsweep-protocol PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(qsweep-protocol PRIVATE QSWEEPPROTOCOL_LIBRARY)


#include(CPackComponent)

#cpack_add_component(runtime)
#cpack_add_component(devel DEPENDS runtime)
#cpack_add_component(sample DEPENDS runtime devel)

#set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
#set(CPACK_PACKAGE_CONTACT "info@example.com")

#set(CPACK_ARCHIVE_COMPONENT_INSTALL On)

#set(CPACK_RPM_COMPONENT_INSTALL On)
#set(CPACK_RPM_PACKAGE_AUTOREQ On)
#set(CPACK_RPM_PACKAGE_AUTOREQPROV Off)
#set(CPACK_RPM_DEVEL_PACKAGE_REQUIRES "blas, lapack, atlas, jsoncpp-devel")
#set(CPACK_RPM_SAMPLE_PACKAGE_REQUIRES "ffmpeg-devel, jsoncpp-devel")

#set(CPACK_DEB_COMPONENT_INSTALL On)
#set(CPACK_DEBIAN_DEVEL_PACKAGE_DEPENDS "libopenblas-base, libblas3, libjsoncpp-dev, libjsoncpp1, libopenblas-dev")
#set(CPACK_DEBIAN_SAMPLE_PACKAGE_DEPENDS "libavformat-dev, libavcodec-dev, libswscale-dev, libswresample-dev, libavutil-dev, libopenblas-dev")

#include(CPack)
