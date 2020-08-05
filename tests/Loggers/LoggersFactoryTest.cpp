#include "LoggersFactoryTest.h"

#include <BeastEngine/Loggers/LoggersTypes.h>
#include <BeastEngine/Loggers/LoggersFactory.h>

namespace be::tests::unit
{
    TEST_F(LoggersFactoryTest, CreateWillReturnConsoleLoggerWhenConsoleLoggerPassedAsType)
    {
        const auto expectedLoggerName = "console_logger";
        const auto loggerType = LoggerType::LOGGER_CONSOLE;

        const auto result = internals::LoggersFactory::Create(loggerType);
        ASSERT_STREQ(expectedLoggerName, result->name().c_str());
    }

    TEST_F(LoggersFactoryTest, CreateWillReturnTheSameInstanceIfCalledMultipleTimesWithTheSameParameter)
    {
        const auto loggerType = LoggerType::LOGGER_CONSOLE;

        const auto firstLoggerInstance = internals::LoggersFactory::Create(loggerType);
        const auto secondLoggerInstance = internals::LoggersFactory::Create(loggerType);

        ASSERT_EQ(firstLoggerInstance, secondLoggerInstance);
    }

    TEST_F(LoggersFactoryTest, CreateWillReturnNotEmptyLoggerWhenCalledWithValidLoggerTypePassed)
    {
        const auto actualLogger = internals::LoggersFactory::Create(LoggerType::LOGGER_CONSOLE);
        ASSERT_TRUE(actualLogger != nullptr);
    }

    TEST_F(LoggersFactoryTest, CreateWillReturnNullptrIfInvalidTypePassed)
    {
        const auto invalidLoggerType = 0xFFFF;
        const auto result = internals::LoggersFactory::Create(static_cast<LoggerType>(invalidLoggerType));

        ASSERT_TRUE(result == nullptr);
    }
} // namespace be::tests::unit
