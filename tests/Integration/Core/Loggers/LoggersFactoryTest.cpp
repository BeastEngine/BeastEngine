#include "LoggersFactoryTest.h"

#include <BeastEngine/Core/Loggers/LoggersTypes.h>
#include <BeastEngine/Core/Loggers/LoggersFactory.h>

#include <spdlog/sinks/basic_file_sink.h>

namespace be::tests::integration
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

        std::string filename = TempFilePath() + "Log.txt";

        const auto result = internals::LoggersFactory::Create(loggerType, filename);
        ASSERT_STREQ(expectedLoggerName, result->name().c_str());
    }

    TEST_F(LoggersFactoryTest, CreateWillReturnNullptrIfInvalidTypePassed)
    {
        const auto invalidLoggerType = 0xFFFF;
        const auto result = internals::LoggersFactory::Create(static_cast<LoggerType>(invalidLoggerType), EMPTY_PARAMS);

        ASSERT_TRUE(result == nullptr);
    }

    INSTANTIATE_TEST_SUITE_P(
        LoggerTypeParams,
        LoggersFactoryTest,
        testing::Values(
            LoggersFactoryTestParams{LoggerType::LOGGER_CONSOLE, EMPTY_PARAMS},
            LoggersFactoryTestParams{LoggerType::LOGGER_FILE, LoggersFactoryTest::TempFilePath() + "Log.txt"}
        )
    );

    TEST_P(LoggersFactoryTest, CreateWillReturnTheSameInstanceIfCalledMultipleTimesWithTheSameParameter)
    {
        const LoggersFactoryTestParams testParams = GetParam();
        const auto loggerType = testParams.loggerType;
        const auto additionalParams = testParams.additionalParams;

        const auto firstLoggerInstance = internals::LoggersFactory::Create(loggerType, additionalParams);
        const auto secondLoggerInstance = internals::LoggersFactory::Create(loggerType, additionalParams);

        ASSERT_EQ(firstLoggerInstance, secondLoggerInstance);
    }

    TEST_P(LoggersFactoryTest, CreateWillReturnNotEmptyLoggerWhenCalledWithValidLoggerTypePassed)
    {
        const LoggersFactoryTestParams testParams = GetParam();
        const auto loggerType = testParams.loggerType;
        const auto additionalParams = testParams.additionalParams;

        const auto actualLogger = internals::LoggersFactory::Create(loggerType, additionalParams);
        ASSERT_TRUE(actualLogger != nullptr);
    }
} // namespace be::tests::integration
