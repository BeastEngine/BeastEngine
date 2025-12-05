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
  "Common/Utils/HasherTest.cpp"
  "Common/IdTest.cpp"
  "Input/InputTest.cpp"
  "Graphics/Camera2DTest.cpp"
  "Common/IdAllocatorTest.cpp"
  "Common/IdAllocatorsFactoryTest.cpp"
  "Common/ResultTest.cpp"
)
target_include_directories(BeastCoreUnits PRIVATE "${BeastEngine_SOURCE_DIR}/tests")
set_target_properties(BeastCoreUnits PROPERTIES OUTPUT_NAME "BeastCoreUnitTest")

find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(BeastCoreUnits PRIVATE BE::core GTest::gtest GTest::gmock spdlog::spdlog)

be_set_compiler_options(BeastCoreUnits)

include(GoogleTest)
gtest_discover_tests(BeastCoreUnits)
