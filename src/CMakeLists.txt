include("${PROJECT_CONFIG_PATH}/beastengine/config.cmake")

if(WIN32)
	list(APPEND BEAST_SRC_LIST "${BEAST_SRC_DIR}/BeastEngine/beastengine.dll.manifest")
endif()

add_library(${BEAST_LIB_TARGET_NAME} SHARED "${BEAST_SRC_LIST}" "${BEAST_HEADERS_LIST}")
target_include_directories(${BEAST_LIB_TARGET_NAME} PUBLIC "${BEAST_INCLUDE_DIR}")
target_compile_features(${BEAST_LIB_TARGET_NAME} PUBLIC cxx_std_17)

# Add linking to libraries from Conan
target_link_libraries(${BEAST_LIB_TARGET_NAME} PUBLIC CONAN_PKG::spdlog CONAN_PKG::glm)

# Group files into proper folders - for IDE
GROUP_FILES("${BEAST_HEADERS_LIST}" "${BEAST_INCLUDE_DIR}")
GROUP_FILES("${BEAST_SRC_LIST}" "${BEAST_SRC_DIR}")

target_compile_definitions(
	${BEAST_LIB_TARGET_NAME}
	PRIVATE
		# Set debugging and assertions macros when bulding with debug info
		$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:${BEAST_DEBUGGING_DEFINES}>
)

if(WIN32)
	target_compile_definitions(${BEAST_LIB_TARGET_NAME} PUBLIC BE_PLATFORM_WINDOWS)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${BEAST_LIB_TARGET_NAME} PRIVATE /EHsc /W4 /c)
endif()

install(TARGETS ${BEAST_LIB_TARGET_NAME} DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

configure_file(
	"${BEAST_INCLUDE_DIR}/BeastEngine/Core/Versions.h.in"
	"${BEAST_INCLUDE_DIR}/BeastEngine/Core/Versions.h"
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
