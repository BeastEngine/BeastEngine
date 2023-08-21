set(
    BE_HEADERS_LIST
    "BeastEngineTest.h"
    "Loggers/FileLoggerTest.h"
    "Windows/Win32/Win32KeyboardEventsTest.h"
    "Windows/Win32/Win32MouseEventsTest.h"
    "Windows/Win32/Win32TestSetup.h"
    "Windows/Win32/Win32WindowTest.h"
    "Ecs/WorldTest.h"
    
)

set(
    BE_SRC_LIST
    "BeastEngineTest.cpp"
    "Loggers/FileLoggerTest.cpp"
    "Windows/Win32/Win32KeyboardEventsTest.cpp"
    "Windows/Win32/Win32MouseEventsTest.cpp"
    "Windows/Win32/Win32WindowTest.cpp"
    "main.cpp"
    "Ecs/WorldTest.cpp"
    "Ecs/ViewTest.cpp"
)

add_executable(BeastCoreIntegration "${BE_SRC_LIST}" "${BE_HEADERS_LIST}")
target_include_directories(BeastCoreIntegration PRIVATE "${BeastEngine_SOURCE_DIR}/tests")

find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(
    BeastCoreIntegration
        PRIVATE
            BE::core
            GTest::gtest
            spdlog::spdlog
)

if(WIN32 AND BUILD_WIN32_TESTS)
    target_compile_definitions(BeastCoreIntegration PRIVATE BE_ENABLE_WINDOWS_TESTS)
endif()

be_set_compiler_options(BeastCoreIntegration)

include(GoogleTest)
gtest_discover_tests(BeastCoreIntegration)
