include("${PROJECT_CONFIG_PATH}/beastengine/config.cmake")

SET(BE_TESTS_UNIT_TARGET_NAME BEASTUnits)

set(BE_TESTS_UNIT_INCLUDE_DIR "${BeastEngine_SOURCE_DIR}/tests")
set(BE_TESTS_UNIT_SRC_DIR "${BeastEngine_SOURCE_DIR}/tests")

set(
	BE_TESTS_UNIT_HEADERS_LIST
	"Core/Events/KeyboardEventsTest.h"
    "Core/Events/MouseEventsTest.h"
    "Core/BeastEngineTest.h"
    "Core/DataStructuresTests.h"
    "TestSetup.h"
    "Core/Loggers/LoggerTest.h"
    "Core/Loggers/LoggersFactoriesTests.h"
    "TestCase.h"
)
set(
	BE_TESTS_UNIT_SRC_LIST
	"main.cpp"
    "Core/Events/KeyboardEventsTest.cpp"
    "Core/Events/MouseEventsTest.cpp"
    "Core/BeastEngineTest.cpp"
    "Core/DataStructuresTests.cpp"
    "Core/Loggers/LoggerTest.cpp"
    "Core/Loggers/LoggersFactoriesTests.cpp"
)

add_executable(${BE_UNIT_TESTS_TARGET_NAME} "${BE_TESTS_UNIT_SRC_LIST}" "${BE_TESTS_UNIT_HEADERS_LIST}")
target_include_directories(${BE_UNIT_TESTS_TARGET_NAME} PRIVATE "${BE_TESTS_UNIT_INCLUDE_DIR}")

target_link_libraries(
	${BE_UNIT_TESTS_TARGET_NAME}
		PRIVATE
		BE::beast
		CONAN_PKG::gtest
		CONAN_PKG::spdlog
)

# Group files into proper folders - for IDE
group_files("${BE_TESTS_UNIT_HEADERS_LIST}" "${BE_TESTS_UNIT_INCLUDE_DIR}")
group_files("${BE_TESTS_UNIT_SRC_LIST}" "${BE_TESTS_UNIT_SRC_DIR}")

target_precompile_headers(
	${BE_UNIT_TESTS_TARGET_NAME}
	PRIVATE
		<string>
		<vector>
		<memory>
		<cstdint>
		<array>
)

set_compiler_options(${BE_UNIT_TESTS_TARGET_NAME})
add_test(NAME ${BE_UNIT_TESTS_TARGET_NAME} COMMAND ${BE_UNIT_TESTS_TARGET_NAME})
