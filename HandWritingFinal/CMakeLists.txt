cmake_minimum_required(VERSION 3.10)
get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME)
project(${CURRENT_DIR_NAME})

set(CMAKE_CXX_STANDARD 11)
if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets Gui Sql REQUIRED)
# Source directories
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CORE_DIR "${SOURCE_DIR}/core")
set(UI_DIR "${SOURCE_DIR}/ui")

# Collect files
file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB RCC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*qrc")
file(GLOB RCC_FILES 
    "${SOURCE_DIR}/resources/resources.qrc"
    "${UI_DIR}/*.ui"
)
file(GLOB CORE_FILES 
    "${CORE_DIR}/*.cpp"
    "${CORE_DIR}/*.h"
)
file(GLOB UI_SOURCE_FILES 
    "${UI_DIR}/*.cpp"
    "${UI_DIR}/*.h"
    "${CMAKE_CURRENT_BINARY_DIR}/*.h"
)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RCC_FILES})
source_group("Core Files" FILES ${CORE_FILES})
source_group("UI Files" FILES ${UI_SOURCE_FILES})

add_executable(${PROJECT_NAME} WIN32 
    ${CORE_FILES}
    ${UI_SOURCE_FILES}
    ${UI_FILES}
    ${RCC_FILES}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} 
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Sql
)