set(BE_TESTS_UNIT_INCLUDE_DIR "${BeastEngine_SOURCE_DIR}/tests")
set(BE_TESTS_UNIT_SRC_DIR "${BeastEngine_SOURCE_DIR}/tests")

set(BE_TESTS_UNIT_HEADERS_PREFIX "${BE_TESTS_UNIT_INCLUDE_DIR}/BeastEngine/Unit/")
set(BE_TESTS_UNIT_SRC_PREFIX "${BE_TESTS_UNIT_SRC_DIR}/BeastEngine/Unit/")

set(
	BE_HEADERS_LIST
	"Events/KeyboardEventsTest.h"
	"Events/MouseEventsTest.h"
	"BeastEngineTest.h"
	"DataStructuresTests.h"
	"Loggers/ConsoleLoggerTest.h"
	"Loggers/LoggerTest.h"
	
 "Ecs/SystemsSchedulerTest.cpp")
set(
	BE_SRC_LIST
	"BeastEngineTest.cpp"
	"DataStructuresTests.cpp"
	"Events/KeyboardEventsTest.cpp"
	"Events/MouseEventsTest.cpp"
	"Loggers/ConsoleLoggerTest.cpp"
	"Loggers/LoggerTest.cpp"
	"main.cpp"
	
 "Ecs/SystemsSchedulerTest.h")

add_executable(BeastCoreUnits "${BE_SRC_LIST}" "${BE_HEADERS_LIST}" "Utils/HasherTest.cpp" "Utils/StringTest.cpp")
target_include_directories(BeastCoreUnits PRIVATE "${BeastEngine_SOURCE_DIR}/tests")

find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(
	BeastCoreUnits
		PRIVATE
		BE::core
		GTest::gtest
		GTest::gmock
		spdlog::spdlog
)

be_set_compiler_options(BeastCoreUnits)

include(GoogleTest)
gtest_discover_tests(BeastCoreUnits)
