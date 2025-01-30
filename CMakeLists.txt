cmake_minimum_required(VERSION 3.20)

project(Widgetry
    DESCRIPTION "The frontend-oriented C++ UI framework"
    VERSION     1.0.0
    LANGUAGES   CXX C
)

option(WIDGETRY_BUILD_TESTS "Build tests" OFF)

option(BUILD_SHARED_LIBS "" OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake/Utils.cmake)
include(cmake/FindHelper.cmake)

find_package(Qt REQUIRED COMPONENTS Core Gui Widgets)

find(Jsoner REPOSITORY "https://github.com/commander15/Jsoner")

add_subdirectory(src)

if (WIDGETRY_BUILD_TESTS)
    find_package(GTest REQUIRED)
    find_package(Qt REQUIRED COMPONENTS Test)

    add_subdirectory(tests)
endif()

if (TRUE)
    add_subdirectory(examples)
endif()
