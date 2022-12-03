#include <Unit/Beast/Core/Loggers/ConsoleLoggerTest.h>

#include <Beast/Core/Loggers/LoggersFactories.h>

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
} // namespace be::tests::unit
