cmake_minimum_required(VERSION 3.19)

project(qsweep-server
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "qsweep-server"
    HOMEPAGE_URL https://github.com/west151/qsweep-system
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE EXT_HEADER
    ../qsweep-protocol/*.h
)
file(GLOB_RECURSE EXT_SOURCES
    ../qsweep-protocol/*.cpp
)

find_package(Qt6 COMPONENTS
    Core
    Network
    Mqtt
REQUIRED)

add_executable(${PROJECT_NAME}
    qsweep-server.cpp
    core_sweep.cpp
    core_sweep.h
    constant.h
    hackrf_info.h
    hackrf_info.cpp
    worker/parser_worker.cpp
    worker/parser_worker.h
    worker/spectrum_native_worker.cpp
    worker/spectrum_native_worker.h
    worker/spectrum_process_worker.cpp
    worker/spectrum_process_worker.h
    worker/state_worker.cpp
    worker/state_worker.h
    worker/system_monitor_worker.cpp
    worker/system_monitor_worker.h
    settings/server_settings.cpp
    settings/server_settings.h
    ${EXT_HEADER}
    ${EXT_SOURCES}
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Mqtt
    )

    target_include_directories(${PROJECT_NAME}
        PRIVATE
        worker
        settings
        ../qsweep-protocol
        ../include/mingw64/hackrf/2018.01.1
        ../include/mingw64/fftw/3.3.10
    )
else()
    # fftw
    find_package(PkgConfig REQUIRED)
    pkg_search_module(FFTW3F REQUIRED fftw3f IMPORTED_TARGET)
    include_directories(PkgConfig::FFTW3F)
    # hackrf
    pkg_search_module(HACKRF REQUIRED libhackrf IMPORTED_TARGET)
    include_directories(PkgConfig::HACKRF)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Mqtt
        PkgConfig::FFTW3F
        PkgConfig::HACKRF
    )

    target_include_directories(${PROJECT_NAME}
        PRIVATE
        worker
        settings
        ../qsweep-protocol
    )
endif()

install(TARGETS ${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
)

#
# CPack
#
# cmake --build . --target package
#

# rpm
#set(CPACK_GENERATOR "RPM")
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "gis1501")
#set(CPACK_PACKAGE_VENDOR "Company")
#set(CPACK_RPM_COMPONENT_INSTALL ON)
#set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/bin/qsweep-system/${PROJECT_NAME}")
#set(CPACK_RPM_POST_TRANS_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/scripts/rpm/control/posttrans)
#set(CPACK_RPM_PACKAGE_DESCRIPTION "qsweep-system: qsweep-server")
#set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME})
#INCLUDE(CPack)

# make deb
#
if(MAKE_DEB_QSWEEP_SERVER)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/packaging/debian-control/postrm.in"
    "${CMAKE_CURRENT_BINARY_DIR}/packaging/debian-control/postrm" @ONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/packaging/debian-control/postinst.in"
    "${CMAKE_CURRENT_BINARY_DIR}/packaging/debian-control/postinst" @ONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/packaging/debian-control/preinst.in"
    "${CMAKE_CURRENT_BINARY_DIR}/packaging/debian-control/preinst" @ONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/packaging/debian-control/prerm.in"
    "${CMAKE_CURRENT_BINARY_DIR}/packaging/debian-control/prerm" @ONLY)

set(CPACK_GENERATOR DEB)
set(CPACK_DEBIAN_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "gis1501@gmail.com")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/bin/${PROJECT_NAME}")
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libc6 (< 2.4)")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_BINARY_DIR}/packaging/debian-control/postrm;${CMAKE_CURRENT_BINARY_DIR}/packaging/debian-control/postinst;${CMAKE_CURRENT_BINARY_DIR}/packaging/debian-control/preinst;${CMAKE_CURRENT_BINARY_DIR}/packaging/debian-control/prerm")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
include(CPack)

endif()
