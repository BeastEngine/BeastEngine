#include "LoggersFactoryTest.h"

#include <BeastEngine/Core/Loggers/LoggersTypes.h>
#include <BeastEngine/Core/Loggers/LoggersFactory.h>

#include <spdlog/sinks/basic_file_sink.h>

namespace be::tests::unit
{
    static const std::string EMPTY_PARAMS = "";

    TEST_F(LoggersFactoryTest, CreateWillReturnConsoleLoggerWhenConsoleLoggerPassedAsType)
    {
        const auto expectedLoggerName = "console_logger";
        const auto loggerType = LoggerType::LOGGER_CONSOLE;

        const auto result = internals::LoggersFactory::Create(loggerType, EMPTY_PARAMS);
        ASSERT_STREQ(expectedLoggerName, result->name().c_str());
    }

    TEST_F(LoggersFactoryTest, CreateWillReturnFileLoggerWhenFileLoggerPassedAsType)
    {
        const auto expectedLoggerName = "file_logger";
        const auto loggerType = LoggerType::LOGGER_FILE;
        const std::string filename = "Log.txt";

        const auto result = internals::LoggersFactory::Create(loggerType, filename);
        ASSERT_STREQ(expectedLoggerName, result->name().c_str());
    }

    TEST_F(LoggersFactoryTest, CreateWillReturnNullptrIfInvalidTypePassed)
    {
        const auto invalidLoggerType = 0xFFFF;
        const auto result = internals::LoggersFactory::Create(static_cast<LoggerType>(invalidLoggerType), EMPTY_PARAMS);

        ASSERT_TRUE(result == nullptr);
    }

    TEST_P(LoggersFactoryTest, CreateWillReturnTheSameInstanceIfCalledMultipleTimesWithTheSameParameter)
    {
        const auto loggerType = GetParam();

        const auto firstLoggerInstance = internals::LoggersFactory::Create(loggerType, EMPTY_PARAMS);
        const auto secondLoggerInstance = internals::LoggersFactory::Create(loggerType, EMPTY_PARAMS);

        ASSERT_EQ(firstLoggerInstance, secondLoggerInstance);
    }

    TEST_P(LoggersFactoryTest, CreateWillReturnNotEmptyLoggerWhenCalledWithValidLoggerTypePassed)
    {
        const auto loggerType = GetParam();
        const auto actualLogger = internals::LoggersFactory::Create(loggerType, EMPTY_PARAMS);
        ASSERT_TRUE(actualLogger != nullptr);
    }

    INSTANTIATE_TEST_SUITE_P(
        LoggerTypeParams,
        LoggersFactoryTest,
        testing::Values(LoggerType::LOGGER_CONSOLE, LoggerType::LOGGER_FILE)
    );
} // namespace be::tests::unit
