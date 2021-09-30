SET(BE_TESTS_UNIT_TARGET_NAME BEASTUnits)

set(BE_TESTS_UNIT_INCLUDE_DIR "${BeastEngine_SOURCE_DIR}/tests")
set(BE_TESTS_UNIT_SRC_DIR "${BeastEngine_SOURCE_DIR}/tests")

set(BE_TESTS_UNIT_HEADERS_PREFIX "${BE_TESTS_UNIT_INCLUDE_DIR}/BeastEngine/Unit/")
set(BE_TESTS_UNIT_SRC_PREFIX "${BE_TESTS_UNIT_SRC_DIR}/BeastEngine/Unit/")

set(
	BE_TESTS_UNIT_HEADERS_LIST
	"Core/Events/KeyboardEventsTest.h"
	"Core/Events/MouseEventsTest.h"
	"Core/BeastEngineTest.h"
	"Core/DataStructuresTests.h"
	"Core/Loggers/ConsoleLoggerTest.h"
	"Core/Loggers/LoggerTest.h"
)
set(
	BE_TESTS_UNIT_SRC_LIST
	"Core/BeastEngineTest.cpp"
	"Core/DataStructuresTests.cpp"
	"Core/Events/KeyboardEventsTest.cpp"
	"Core/Events/MouseEventsTest.cpp"
	"Core/Loggers/ConsoleLoggerTest.cpp"
	"Core/Loggers/LoggerTest.cpp"
	"main.cpp"
)

list(TRANSFORM BE_TESTS_UNIT_HEADERS_LIST PREPEND ${BE_TESTS_UNIT_HEADERS_PREFIX})
list(TRANSFORM BE_TESTS_UNIT_SRC_LIST PREPEND ${BE_TESTS_UNIT_SRC_PREFIX})

list(APPEND BE_TESTS_UNIT_HEADERS_LIST "${BE_TESTS_UNIT_INCLUDE_DIR}/TestSetup.h")

add_executable(${BE_TESTS_UNIT_TARGET_NAME} "${BE_TESTS_UNIT_SRC_LIST}" "${BE_TESTS_UNIT_HEADERS_LIST}")
target_include_directories(${BE_TESTS_UNIT_TARGET_NAME} PRIVATE "${BE_TESTS_UNIT_INCLUDE_DIR}")

target_link_libraries(
	${BE_TESTS_UNIT_TARGET_NAME}
		PRIVATE
		BE::beast
		CONAN_PKG::gtest
		CONAN_PKG::spdlog
)

# Group files into proper folders - for IDE
be_group_files("${BE_TESTS_UNIT_HEADERS_LIST}" "${BE_TESTS_UNIT_INCLUDE_DIR}")
be_group_files("${BE_TESTS_UNIT_SRC_LIST}" "${BE_TESTS_UNIT_SRC_DIR}")

be_set_compiler_options(${BE_TESTS_UNIT_TARGET_NAME})
add_test(NAME ${BE_TESTS_UNIT_TARGET_NAME} COMMAND ${BE_TESTS_UNIT_TARGET_NAME})
