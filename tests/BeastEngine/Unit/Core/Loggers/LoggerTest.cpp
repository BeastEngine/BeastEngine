#include "LoggerTest.h"

#include "spdlog/details/null_mutex.h"

namespace be::tests::unit
{
    TEST_F(LoggerTest, ConstructorWillThrowExceptionWhenEmptyLoggerImplementationPassed)
    {
        LoggerImpl emptyImplementation = nullptr;
        ASSERT_THROW(LoggerDummy{emptyImplementation}, std::invalid_argument);
    }

    SharedPtr<spdlog::logger> LoggerTest::PrepareLogFunctionTests()
    {
        SharedPtr<spdlog::sinks::sink> sink = CreateSharedPtr<SinkMock>();
        auto loggerImpl = CreateSharedPtr<spdlog::logger>("loggerName", sink);

        auto expectedSink = static_cast<SinkMock*>(&*sink);
        EXPECT_CALL(*expectedSink, log).Times(1);

        return loggerImpl;
    }

    TEST_F(LoggerTest, LogInfoWillUseLoggerPassedViaConstructor)
    {
        const std::string messageToLog = "Info";
        auto preparedLogger = PrepareLogFunctionTests();

        auto sut = LoggerDummy(std::move(preparedLogger));
        sut.LogInfo(messageToLog);
    }

    TEST_F(LoggerTest, LogWarningWillUseLoggerPassedViaConstructor)
    {
        const std::string messageToLog = "Warning";
        auto preparedLogger = PrepareLogFunctionTests();

        auto sut = LoggerDummy(std::move(preparedLogger));
        sut.LogWarning(messageToLog);
    }

    TEST_F(LoggerTest, LogErrorWillUseLoggerPassedViaConstructor)
    {
        const std::string messageToLog = "Error";
        auto preparedLogger = PrepareLogFunctionTests();

        auto sut = LoggerDummy(std::move(preparedLogger));
        sut.LogError(messageToLog);
    }

    TEST_F(LoggerTest, LogFatalErrorWillUseLoggerPassedViaConstructor)
    {
        const std::string messageToLog = "Fatal Error";
        auto preparedLogger = PrepareLogFunctionTests();

        auto sut = LoggerDummy(std::move(preparedLogger));
        sut.LogFatalError(messageToLog);
    }
} // namespace be::tests::unit
