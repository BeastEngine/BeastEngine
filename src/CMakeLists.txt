include("${PROJECT_CONFIG_PATH}/beastengine/config.cmake")

set(
	BEAST_HEADERS_LIST
	"Core/BeastEngine.h"
    "Core/Loggers/StaticLogger.h"
    "Core/Types.h"
    "Core/Helpers.h"
    "Core/Logging.h"
    "Core/Assertions.h"
    "Core/Versions.h"
    "BeastEngine.h"
    "Core/Windows/IWindow.h"
    "Core/Events/KeyboardEvents.h"
    "Core/Input/InputCodes.h"
    "Core/Events/Events.h"
    "Core/Events/MouseEvents.h"
    "Core/Windows/AWindow.h"
    "Core/Windows/IWindowFactory.h"
    "Core/Windows/WindowFactory.h"
    "EntryPoint.h"
    "Core/PlatformSetup.h"
    "Core/Application.h"
    "Core/Windows/Windows/WindowsWindow.h"
    "Core/DataStructures.h"
    "Core/Loggers/Logger.h"
    "Core/Loggers/LoggersFactories.h"
    "Core/Debugging.h"
)
set(
	BEAST_SRC_LIST
	"Core/BeastEngine.cpp"
    "Core/Windows/WindowFactory.cpp"
    "Core/Windows/Windows/WindowsWindow.cpp"
    "Core/Application.cpp"
    "Core/Loggers/Logger.cpp"
    "Core/Loggers/LoggersFactories.cpp"
)

if(WIN32)
	list(APPEND BEAST_SRC_LIST "beastengine.dll.manifest")
endif()

list(TRANSFORM BEAST_HEADERS_LIST PREPEND ${BEAST_HEADERS_PREFIX})
list(TRANSFORM BEAST_SRC_LIST PREPEND ${BEAST_SRC_PREFIX})

add_library(${BEAST_LIB_TARGET_NAME} SHARED "${BEAST_SRC_LIST}" "${BEAST_HEADERS_LIST}")
target_include_directories(${BEAST_LIB_TARGET_NAME} PUBLIC "${BEAST_INCLUDE_DIR}")
target_compile_features(${BEAST_LIB_TARGET_NAME} PUBLIC cxx_std_17)

# Add linking to libraries from Conan
target_link_libraries(
	${BEAST_LIB_TARGET_NAME}
		PUBLIC
		CONAN_PKG::spdlog
		CONAN_PKG::glm
		CT::common_utils
)

# TODO: Transform it into an option. If BE_ENABLE_DEBUGGING is ON
set(BEAST_DEBUGGING_DEFINES BE_DEBUGGING_INFO_ENABLED BE_ASSERTIONS_ENABLED)
target_compile_definitions(
	${BEAST_LIB_TARGET_NAME}
	PRIVATE
		# Set debugging and assertions macros when bulding with debug info
		$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:${BEAST_DEBUGGING_DEFINES}>
)

if(WIN32)
	target_compile_definitions(${BEAST_LIB_TARGET_NAME} PUBLIC BE_PLATFORM_WINDOWS)
endif()

set_compiler_options(${BEAST_LIB_TARGET_NAME})

configure_file(
	"${BEAST_HEADERS_PREFIX}/Core/Versions.h.in"
	"${BEAST_HEADERS_PREFIX}/Core/Versions.h"
)

target_precompile_headers(
	${BEAST_LIB_TARGET_NAME}
	PRIVATE
		<string>
		<vector>
		<memory>
		<cstdint>
		<array>
)

# Group files into proper folders - for IDE
GROUP_FILES("${BEAST_HEADERS_LIST}" "${BEAST_INCLUDE_DIR}")
GROUP_FILES("${BEAST_SRC_LIST}" "${BEAST_SRC_DIR}")

add_library(BE::beast ALIAS ${BEAST_LIB_TARGET_NAME})
