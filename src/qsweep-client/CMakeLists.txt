cmake_minimum_required(VERSION 3.19)

project(qsweep-client
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "qsweep-client"
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
    Quick
    StateMachine
    QuickControls2
REQUIRED
)

add_executable(${PROJECT_NAME}
    ${HEADER}
    ${SOURCES}
    qml.qrc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core Qt6::Network Qt6::Mqtt Qt6::Sql Qt6::Quick Qt6::StateMachine)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    template
    model
    chart
    database
    spectr
    ui_interface
    settings
    ../qsweep-protocol
)

#install(TARGETS ${PROJECT_NAME}
#    COMPONENT ${PROJECT_NAME}
#)

#include(GNUInstallDirs)

#install(TARGETS ${PROJECT_NAME}
#    RUNTIME DESTINATION
#)
