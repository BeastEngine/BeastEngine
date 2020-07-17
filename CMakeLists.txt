cmake_minimum_required(VERSION 3.16)

set(BEAST_CONFIG_PATH "${CMAKE_SOURCE_DIR}/cmake/config")

include("${BEAST_CONFIG_PATH}/config.cmake")
include(cmake/functions.cmake)

CHECK_CONFIG() # Check if all required variables are set in config

project(${BEAST_PROJECT_NAME}
		VERSION "${BEAST_VERSION_MAJOR}.${BEAST_VERSION_MINOR}.${BEAST_VERSION_PATCH}"
		DESCRIPTION "2D focused game engine"
		HOMEPAGE_URL https://github.com/BeastEngine/BeastEngine
		LANGUAGES CXX)

include("${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake")
conan_basic_setup(TARGETS) # Configure Conan, but do not let it override output directories

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_INSTALL_PREFIX "${BeastEngine_SOURCE_DIR}")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # Export all DLL symbols by default
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(CMAKE_CXX_CLANG_TIDY ON)

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	option(BUILD_TESTS "Build the tests" OFF)
endif()

add_subdirectory(src) # Library
add_subdirectory(apps) # Executable

# Add tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTS)
	add_subdirectory(tests)
endif()

# Set the VS startup project to the executable
SET_STARTUP_PROJECT(${BEAST_EXE_TARGET_NAME})
