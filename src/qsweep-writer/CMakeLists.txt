cmake_minimum_required(VERSION 3.19)

project(qsweep-writer
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "qsweep-writer"
    HOMEPAGE_URL https://github.com/west151/qsweep-system
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE HEADER
    *.h
    ../qsweep-protocol/*.h
)
file(GLOB_RECURSE SOURCES
    *.cpp
    ../qsweep-protocol/*.cpp
)

find_package(Qt6 COMPONENTS
    Core
    Network
    Mqtt
    Sql
REQUIRED
)

add_executable(${PROJECT_NAME}
    ${HEADER}
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core Qt6::Network Qt6::Mqtt Qt6::Sql)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    database
    provider
    ../qsweep-protocol
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
