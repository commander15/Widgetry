cmake_minimum_required(VERSION 3.20)

project(Widgetry
    DESCRIPTION "The frontend-oriented C++ UI framework"
    VERSION     1.0.0
    LANGUAGES   CXX C
)

option(WIDGETRY_BUILD_TESTS    "Build tests"    OFF)
option(WIDGETRY_BUILD_EXAMPLES "Build examples" OFF)

option(BUILD_SHARED_LIBS "" OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake/Utils.cmake)

find_package(Qt REQUIRED COMPONENTS Core Gui Widgets)
find_package(Jsoner REQUIRED)

add_subdirectory(src)

if (WIDGETRY_BUILD_TESTS)
    find_package(GTest REQUIRED)
    find_package(Qt REQUIRED COMPONENTS Test)

    add_subdirectory(tests)
endif()

if (WIDGETRY_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
