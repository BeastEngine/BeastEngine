include("${PROJECT_CONFIG_PATH}/lab/unit/config.cmake")

add_executable(${BEAST_UNIT_TESTS_TARGET_NAME} "${LAB_UNIT_SRC_LIST}" "${LAB_UNIT_HEADERS_LIST}")
target_include_directories(${BEAST_UNIT_TESTS_TARGET_NAME} PRIVATE "${LAB_UNIT_INCLUDE_DIR}")

target_link_libraries(${BEAST_UNIT_TESTS_TARGET_NAME} PRIVATE ${BEAST_LIB_TARGET_NAME})
target_link_libraries(${BEAST_UNIT_TESTS_TARGET_NAME} PRIVATE CONAN_PKG::gtest CONAN_PKG::spdlog)

# Group files into proper folders - for IDE
GROUP_FILES("${LAB_UNIT_HEADERS_LIST}" "${LAB_UNIT_INCLUDE_DIR}")
GROUP_FILES("${LAB_UNIT_SRC_LIST}" "${LAB_UNIT_SRC_DIR}")

install(TARGETS ${BEAST_UNIT_TESTS_TARGET_NAME} DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

target_precompile_headers(
	${BEAST_UNIT_TESTS_TARGET_NAME}
	PRIVATE
		<string>
		<vector>
		<memory>
		<cstdint>
		<array>
)
