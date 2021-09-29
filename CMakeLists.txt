cmake_minimum_required(VERSION 3.16)

set(PROJECT_CMAKE_FILES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(PROJECT_CONFIG_PATH "${PROJECT_CMAKE_FILES_PATH}/config")

include("${PROJECT_CONFIG_PATH}/config.cmake")
include("${PROJECT_CMAKE_FILES_PATH}/functions.cmake")
include(GenerateExportHeader)

project(
	${PROJECT_MAIN_NAME}
	VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
	DESCRIPTION "${PROJECT_DESCRIPTION}"
	HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}"
	LANGUAGES CXX
)

include("${CMAKE_CURRENT_SOURCE_DIR}/conan/build/conanbuildinfo_multi.cmake")
conan_basic_setup(TARGETS) # Configure Conan, but do not let it override output directories

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_INSTALL_PREFIX "${${PROJECT_MAIN_NAME}_SOURCE_DIR}")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # Export all DLL symbols by default
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Export compilation db
	set(CMAKE_CXX_CLANG_TIDY ON)

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	option(BUILD_TESTS "Build the tests" ON)
	option(BUILD_UNIT_TESTS "Build unit tests" ON)
	option(BUILD_INTEGRATION_TESTS "Build integration tests" ON)
	option(GROUP_FILES_SEPARATE "Group source and header files into different sub-categories inside the IDE" OFF)

	option(BE_ENABLE_DEBUG_INFO "Should debugging functionalities be enabled" ON)
	option(BE_ENABLE_ASSERTIONS "Should assertions be enabled" ON)

	if (BE_ENABLE_DEBUG_INFO)
		set(CT_COMMON_ENABLE_DEBUG TRUE)
	endif()
endif()

add_subdirectory(vendor) # Vendors' code
add_subdirectory(src) # Library

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_subdirectory(apps) # Add executable

	if (BUILD_TESTS)
		add_subdirectory(tests) # Add tests
	endif()
endif()

# Set the VS startup project to the executable
set_startup_project(${PROJECT_IDE_STARTUP_PROJECT})
