include("${PROJECT_CONFIG_PATH}/beastengine/config.cmake")

add_library(${BEAST_LIB_TARGET_NAME} SHARED "${BEAST_SRC_LIST}" "${BEAST_HEADERS_LIST}")
target_include_directories(${BEAST_LIB_TARGET_NAME} PUBLIC "${BEAST_INCLUDE_DIR}")
target_compile_features(${BEAST_LIB_TARGET_NAME} PUBLIC cxx_std_17)

# Add linking to libraries from Conan
target_link_libraries(${BEAST_LIB_TARGET_NAME} PUBLIC CONAN_PKG::spdlog)

# Group files into proper folders - for IDE
GROUP_FILES("${BEAST_HEADERS_LIST}" "${BEAST_INCLUDE_DIR}")
GROUP_FILES("${BEAST_SRC_LIST}" "${BEAST_SRC_DIR}")

target_compile_definitions(
	${BEAST_LIB_TARGET_NAME} PRIVATE
	$<$<CONFIG:Debug>:BE_DEBUGGING_INFO_ENABLED BE_ASSERTIONS_ENABLED>
	$<$<CONFIG:RelWithDebInfo>:BE_DEBUGGING_INFO_ENABLED BE_ASSERTIONS_ENABLED>
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${BEAST_LIB_TARGET_NAME} PRIVATE /EHsc /W4 /c)
endif()

install(TARGETS ${BEAST_LIB_TARGET_NAME} DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

configure_file(
	"${BEAST_INCLUDE_DIR}/BeastEngine/Definitions/Versions.h.in"
	"${BEAST_INCLUDE_DIR}/BeastEngine/Definitions/Versions.h"
)
