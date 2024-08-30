set(BE_TESTS_UNIT_INCLUDE_DIR "${BeastEngine_SOURCE_DIR}/tests")
set(BE_TESTS_UNIT_SRC_DIR "${BeastEngine_SOURCE_DIR}/tests")

set(BE_TESTS_UNIT_HEADERS_PREFIX "${BE_TESTS_UNIT_INCLUDE_DIR}/BeastEngine/Unit/")
set(BE_TESTS_UNIT_SRC_PREFIX "${BE_TESTS_UNIT_SRC_DIR}/BeastEngine/Unit/")

add_executable(
  BeastCoreUnits
  "DataStructuresTests.cpp"
  "Input/Events/KeyboardEventsTest.cpp"
  "Input/Events/MouseEventsTest.cpp"
  "Loggers/ConsoleLoggerTest.cpp"
  "Loggers/LoggerTest.cpp"
  "main.cpp"
  "DataStructuresTests.h"
  "Loggers/ConsoleLoggerTest.h"
  "Loggers/LoggerTest.h"
  "Ecs/SystemsSchedulerTest.cpp"
  "Common/Utils/HasherTest.cpp"
  "Common/IdTest.cpp"
  "Input/InputTest.cpp"
  "Ecs/WorldTest.cpp"
  "Ecs/ViewTest.cpp"
  "Ecs/SchedulerTest.cpp"
)
target_include_directories(BeastCoreUnits PRIVATE "${BeastEngine_SOURCE_DIR}/tests")

find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(BeastCoreUnits PRIVATE BE::core GTest::gtest GTest::gmock spdlog::spdlog)

be_set_compiler_options(BeastCoreUnits)

include(GoogleTest)
gtest_discover_tests(BeastCoreUnits)
