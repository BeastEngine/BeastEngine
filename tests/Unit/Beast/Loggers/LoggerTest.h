#pragma once
#include <TestSetup.h>

#include <Beast/Loggers/Logger.h>

#include <spdlog/sinks/base_sink.h>

namespace be::tests::unit
{
    class SinkMock : public spdlog::sinks::sink
    {
    public:
        MOCK_METHOD(void, log, (const spdlog::details::log_msg& msg), (override));
        MOCK_METHOD(void, flush, (), (override));
        MOCK_METHOD(void, set_pattern, (const std::string& pattern), (override));
        MOCK_METHOD(void, set_formatter, (std::unique_ptr<spdlog::formatter> sink_formatter), (override));
    };

    class LoggerTest : public ::testing::Test
    {
    protected:
        auto CreateSinkMock()
        {
            return CreateShared<SinkMock>();
        }

        auto CreateSut(Shared<SinkMock> sink)
        {
            return Logger(CreateShared<spdlog::logger>("loggerName", std::move(sink)));
        }
    };
} // namespace be::tests::unit
