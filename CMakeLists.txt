cmake_minimum_required(VERSION 3.13...3.16)

project(BeastEngine
		VERSION 1.0.0.0
		DESCRIPTION "2D focused game engine"
		HOMEPAGE_URL https://github.com/BeastEngine/BeastEngine
		LANGUAGES CXX)

include(cmake/functions.cmake)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_INSTALL_PREFIX "${BeastEngine_SOURCE_DIR}") # Directory to put all the targets from `install()` commands
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	include(CTest)
endif()

# Add library
add_subdirectory(src)
# Add executables
add_subdirectory(apps)

# Set the VS startup project to the executable if its name was set
get_property(EXECUTABLE_NAME GLOBAL PROPERTY BEAST_EXE_TARGET_NAME SET)
if("${EXECUTABLE_NAME}")
	get_property(PROJECT_NAME GLOBAL PROPERTY BEAST_EXE_TARGET_NAME)
	SET_STARTUP_PROJECT("${PROJECT_NAME}")
endif()