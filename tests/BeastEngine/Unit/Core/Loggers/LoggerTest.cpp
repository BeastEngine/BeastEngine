#include "LoggerTest.h"

#include "spdlog/details/null_mutex.h"

namespace be::tests::unit
{
    TEST_F(LoggerTest, ConstructorWillThrowExceptionWhenEmptyLoggerImplementationPassed)
    {
        ASSERT_THROW(Logger(nullptr), std::invalid_argument);
    }

    TEST_F(LoggerTest, LogInfoWillUseLoggerPassedViaConstructor)
    {       
        auto loggerSink = CreateSinkMock();
        EXPECT_CALL(*loggerSink, log).Times(1);

        auto sut = CreateSut(std::move(loggerSink));
        sut.LogInfo("Info");
    }

    TEST_F(LoggerTest, LogWarningWillUseLoggerPassedViaConstructor)
    {
        auto loggerSink = CreateSinkMock();
        EXPECT_CALL(*loggerSink, log).Times(1);

        auto sut = CreateSut(std::move(loggerSink));
        sut.LogWarning("Warning");
    }

    TEST_F(LoggerTest, LogErrorWillUseLoggerPassedViaConstructor)
    {
        auto loggerSink = CreateSinkMock();
        EXPECT_CALL(*loggerSink, log).Times(1);

        auto sut = CreateSut(std::move(loggerSink));
        sut.LogError("Error");
    }

    TEST_F(LoggerTest, LogFatalErrorWillUseLoggerPassedViaConstructor)
    {
        auto loggerSink = CreateSinkMock();
        EXPECT_CALL(*loggerSink, log).Times(1);

        auto sut = CreateSut(std::move(loggerSink));
        sut.LogFatalError("FatalError");
    }
} // namespace be::tests::unit
