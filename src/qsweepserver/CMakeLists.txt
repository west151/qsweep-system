cmake_minimum_required(VERSION 3.19)

project(qsweep-server VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE HEADER
    *.h
    ../protocol/*.h
)
file(GLOB_RECURSE SOURCES
    *.cpp
    ../protocol/*.cpp
)

find_package(Qt6 COMPONENTS
    Core
    Network
    Mqtt
REQUIRED)

add_executable(${PROJECT_NAME}
    ${HEADER}
    ${SOURCES}
)

# fftw
find_package(PkgConfig REQUIRED)
pkg_search_module(FFTW3F REQUIRED fftw3f IMPORTED_TARGET)
include_directories(PkgConfig::FFTW3F)
# hackrf
pkg_search_module(HACKRF REQUIRED libhackrf IMPORTED_TARGET)
include_directories(PkgConfig::HACKRF)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Network Qt6::Mqtt PkgConfig::FFTW3F PkgConfig::HACKRF)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ../protocol
    /usr/include/libhackrf
)

install(TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Release
    DESTINATION bin)

#install(TARGETS target
#        CONFIGURATIONS Debug
#        RUNTIME DESTINATION Debug/bin)
#install(TARGETS target
#        CONFIGURATIONS Release
#        RUNTIME DESTINATION Release/bin)
