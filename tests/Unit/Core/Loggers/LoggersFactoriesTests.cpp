#include "LoggersFactoriesTests.h"

#include <BeastEngine/Core/Loggers/LoggersFactories.h>

namespace be::tests::unit
{
    TEST_F(ConsoleLoggerTest, CreateWillReturnNotEmptyLoggerImplenetation)
    {
        const std::string loggerName = "console_logger";
        ASSERT_NE(nullptr, ConsoleLogger::Create(loggerName));
    }

    TEST_F(ConsoleLoggerTest, CreateWillReturnValidLoggerWhenCalledMultipleTimesWithTheSameName)
    {
        const std::string loggerName = "console_logger";

        ASSERT_NO_THROW(
            const auto firstLogger = ConsoleLogger::Create(loggerName);
            const auto secondLogger = ConsoleLogger::Create(loggerName);

            ASSERT_NE(nullptr, firstLogger);
            ASSERT_NE(nullptr, secondLogger);
        );
    }

    TEST_F(FileLoggerTest, CreateWillReturnNotEmptyLoggerImplenetation)
    {
        const std::string loggerName = "file_logger";
        const std::string filePath = TempFilePath() + "file_logger.log";

        ASSERT_NE(nullptr, FileLogger::Create(loggerName, filePath));
    }

    TEST_F(FileLoggerTest, CreateWillReturnValidLoggerWhenCalledMultipleTimesWithTheSameName)
    {
        const std::string loggerName = "file_logger";
        const std::string filePath = TempFilePath() + "file_logger.log";

        ASSERT_NO_THROW(
            const auto firstLogger = FileLogger::Create(loggerName, filePath);
            const auto secondLogger = FileLogger::Create(loggerName, filePath);

            ASSERT_NE(nullptr, firstLogger);
            ASSERT_NE(nullptr, secondLogger);
        );
    }
} // namespace be::tests::unit
