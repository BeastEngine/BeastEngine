#pragma once
#include "TestSetup.h"
#include <BeastEngine/Core/Loggers/Logger.h>

#include <spdlog/sinks/base_sink.h>

namespace be::tests::unit
{
    class LoggerDummy : public Logger
    {
    public:
        LoggerDummy(LoggerImpl logger)
            : Logger(logger)
        {
        }
    };

    class SinkMock : public spdlog::sinks::sink
    {
    public:
        SinkMock() = default;

        MOCK_METHOD(void, log, (const spdlog::details::log_msg& msg), (override));
        MOCK_METHOD(void, flush, (), (override));
        MOCK_METHOD(void, set_pattern, (const std::string& pattern), (override));
        MOCK_METHOD(void, set_formatter, (std::unique_ptr<spdlog::formatter> sink_formatter), (override));
    };

    class LoggerTest : public ::testing::Test
    {
    protected:
        SharedPtr<spdlog::logger> PrepareLogFunctionTests();
    };
} // namespace be::tests::unit
