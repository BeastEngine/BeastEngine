SET(BEAST_LIB_TARGET_NAME BeastEngine)

set(BEAST_INCLUDE_DIR "${${PROJECT_MAIN_NAME}_SOURCE_DIR}/include")
set(BEAST_SRC_DIR "${${PROJECT_MAIN_NAME}_SOURCE_DIR}/src")

set(BEAST_HEADERS_PREFIX "${BEAST_INCLUDE_DIR}/${BEAST_LIB_TARGET_NAME}/")
set(BEAST_SRC_PREFIX "${BEAST_SRC_DIR}/${BEAST_LIB_TARGET_NAME}/")


set(
	BEAST_HEADERS_LIST
	"BeastEngine.h"
	"Core/Application.h"
	"Core/Assertions.h"
	"Core/BeastEngine.h"
	"Core/DataStructures.h"
	"Core/Debugging.h"
	"Core/Events/Events.h"
	"Core/Events/KeyboardEvents.h"
	"Core/Events/MouseEvents.h"
	"Core/Input/InputCodes.h"
	"Core/Loggers/Logger.h"
	"Core/Loggers/LoggersFactories.h"
	"Core/Loggers/StaticLogger.h"
	"Core/PlatformSetup.h"
	"Core/Versions.h"
	"Core/Windows/AWindow.h"
	"Core/Windows/IWindow.h"
	"Core/Windows/IWindowFactory.h"
	"Core/Windows/Win32/Win32Window.h"
	"Core/Windows/WindowFactory.h"
	"EntryPoint.h"
)
set(
	BEAST_SRC_LIST
	"Core/Application.cpp"
	"Core/BeastEngine.cpp"
	"Core/Loggers/Logger.cpp"
	"Core/Loggers/LoggersFactories.cpp"
	"Core/Windows/Win32/Win32Window.cpp"
	"Core/Windows/WindowFactory.cpp"
)

if(WIN32)
	list(APPEND BEAST_SRC_LIST "beastengine.dll.manifest")
endif()

list(TRANSFORM BEAST_HEADERS_LIST PREPEND ${BEAST_HEADERS_PREFIX})
list(TRANSFORM BEAST_SRC_LIST PREPEND ${BEAST_SRC_PREFIX})

add_library(${BEAST_LIB_TARGET_NAME} SHARED "${BEAST_SRC_LIST}" "${BEAST_HEADERS_LIST}")
target_include_directories(${BEAST_LIB_TARGET_NAME} PUBLIC "${BEAST_INCLUDE_DIR}")
target_compile_features(${BEAST_LIB_TARGET_NAME} PUBLIC cxx_std_17)

target_link_libraries(
	${BEAST_LIB_TARGET_NAME}
	PUBLIC
		CONAN_PKG::spdlog
		CT::common_utils
		BE::math
)


set(BEAST_DEBUGGING_DEFINES)
if (${BE_ENABLE_DEBUG_INFO})
	list(APPEND BEAST_DEBUGGING_DEFINES BE_DEBUGGING_INFO_ENABLED)
endif()

if (${BE_ENABLE_ASSERTIONS})
	list(APPEND BEAST_DEBUGGING_DEFINES BE_ASSERTIONS_ENABLED)
endif()

target_compile_definitions(${BEAST_LIB_TARGET_NAME} PRIVATE ${BEAST_DEBUGGING_DEFINES})

if(WIN32)
	target_compile_definitions(${BEAST_LIB_TARGET_NAME} PUBLIC BE_PLATFORM_WINDOWS)
endif()

be_set_compiler_options(${BEAST_LIB_TARGET_NAME})

configure_file(
	"${BEAST_HEADERS_PREFIX}/Core/Versions.h.in"
	"${BEAST_HEADERS_PREFIX}/Core/Versions.h"
)

# Group files into proper folders - for IDE
be_group_files("${BEAST_HEADERS_LIST}" "${BEAST_INCLUDE_DIR}")
be_group_files("${BEAST_SRC_LIST}" "${BEAST_SRC_DIR}")

add_library(BE::beast ALIAS ${BEAST_LIB_TARGET_NAME})
