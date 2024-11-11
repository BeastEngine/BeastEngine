cmake_minimum_required(VERSION 3.16)

set(PROJECT_CMAKE_FILES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(PROJECT_CONFIG_PATH "${PROJECT_CMAKE_FILES_PATH}/config")

include("${PROJECT_CMAKE_FILES_PATH}/functions.cmake")
include(GenerateExportHeader)

project(
    "BeastEngine"
    VERSION "0.0.1"
    DESCRIPTION "2D focused game engine"
    HOMEPAGE_URL "https://github.com/BeastEngine/BeastEngine"
    LANGUAGES CXX
)

set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_INSTALL_PREFIX "${${PROJECT_MAIN_NAME}_SOURCE_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # Export all DLL symbols by default
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Export compilation db
    # set(CMAKE_CXX_CLANG_TIDY ON) TODO: This shuld be set to something like: set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-style=file;-checks=*")

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    option(BUILD_TESTS "Build the tests" ON)
    option(BUILD_UNIT_TESTS "Build unit tests" ON)
    option(BUILD_INTEGRATION_TESTS "Build integration tests" ON)
    option(GROUP_FILES_SEPARATE "Group source and header files into different sub-categories inside the IDE" OFF)

    option(BE_ENABLE_DEBUG_INFO "Should debugging functionalities be enabled" ON)
    option(BE_ENABLE_ASSERTIONS "Should assertions be enabled" ON)

    set(BUILD_WIN32_TESTS_VALUE OFF)
    if(WIN32)
        set(BUILD_WIN32_TESTS_VALUE ON)
    endif()
    option(BUILD_WIN32_TESTS "Build integration tests for Win32 components" ${BUILD_WIN32_TESTS_VALUE})

    if (BE_ENABLE_DEBUG_INFO)
        set(BE_COMMON_ENABLE_DEBUG TRUE)
    endif()
endif()

add_subdirectory(extern)
add_subdirectory(src/Beast)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(apps) # Add executable

    if (BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests) # Add tests
    endif()
endif()
