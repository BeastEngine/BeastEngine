#include <Integration/FilesystemTestCase.h>

#include <Beast/Loggers/LoggersFactories.h>
#include <set>

namespace be::tests::integration
{
    class FileLoggerTest : public FilesystemTestCase
    {
    protected:
        void TearDown() override
        {
            // We're destroying all loggers, because spdlog stores handles to files and we want to release those handles
            // in order to delete temporary directory inside the FilesystemTestCase::TearDown()
            for (const auto& name : m_namesRegister)
            {
                FileLogger::Destroy(name);
            }

            m_namesRegister.clear();
            FilesystemTestCase::TearDown();
        }

        auto CreateLogger(const std::string& name, const FilesystemPath& filePath)
        {
            m_namesRegister.insert(name);
            return FileLogger::Create(name, filePath);
        }

    private:
        std::set<std::string> m_namesRegister;
    };

    TEST_F(FileLoggerTest, CreateWillReturnNotEmptyLoggerImplenetation)
    {
        const auto loggerName = "file_logger";
        const auto filePath = GetFullPath("file_logger.log");

        ASSERT_NE(nullptr, CreateLogger(loggerName, filePath));
    }

    TEST_F(FileLoggerTest, CreateWillReturnValidLoggerWhenCalledMultipleTimesWithTheSameName)
    {
        const auto loggerName = "file_logger";
        const auto filePath = GetFullPath("file_logger.log");

        ASSERT_NO_THROW(
            const auto firstLogger = CreateLogger(loggerName, filePath);
            const auto secondLogger = CreateLogger(loggerName, filePath);

            ASSERT_NE(nullptr, firstLogger);
            ASSERT_NE(nullptr, secondLogger);
        );
    }

    TEST_F(FileLoggerTest, CreateWillCreateLogFileUnderGivenFilePathIfFileDoesNotExist)
    {
        const auto loggerName = "file_logger";
        const auto filePath = GetFullPath("file_logger.log");

        bool fileExistenceBefore = fs::exists(filePath);

        const auto logger = CreateLogger(loggerName, filePath);
        bool fileExistenceAfter = fs::exists(filePath);

        ASSERT_TRUE(fileExistenceAfter);
        ASSERT_NE(fileExistenceBefore, fileExistenceAfter);
    }

    TEST_F(FileLoggerTest, CreateWillNotClearLogFileUnderGivenFilePathIfFileAlreadyExists)
    {
        const auto fileContentBefore = "File content";

        const auto loggerName = "file_logger";
        const auto filePath = CreateFile("file_logger.log", fileContentBefore);

        const auto logger = CreateLogger(loggerName, filePath);

        const auto fileContentAfter = ReadFile(filePath);
        ASSERT_EQ(fileContentBefore, fileContentAfter);
    }

    TEST_F(FileLoggerTest, CreateWillIgnoreFilePathIfCalledSecondTimeWithTheSameName)
    {
        const auto loggerName = "file_logger";
        const auto filePath = GetFullPath("file_logger.log");
        const auto otherFilePath = GetFullPath("other_file.log");

        bool otherFileExistenceBefore = fs::exists(otherFilePath);

        const auto logger1 = CreateLogger(loggerName, filePath);
        const auto logger2 = CreateLogger(loggerName, otherFilePath);

        bool otherFileExistenceAfter = fs::exists(otherFilePath);

        ASSERT_FALSE(otherFileExistenceAfter);
        ASSERT_EQ(otherFileExistenceBefore, otherFileExistenceAfter);
    }

    TEST_F(FileLoggerTest, CreateWillCreateNewLogFileIfCalledSecondTimeWithTheSameNameButDestroyedPreviousInstance)
    {
        const auto loggerName = "file_logger";
        const auto filePath = GetFullPath("file_logger.log");
        const auto otherFilePath = GetFullPath("other_file.log");

        bool otherFileExistenceBefore = fs::exists(otherFilePath);

        const auto logger1 = CreateLogger(loggerName, filePath);
        FileLogger::Destroy(loggerName);

        const auto logger2 = CreateLogger(loggerName, otherFilePath);
        bool otherFileExistenceAfter = fs::exists(otherFilePath);

        ASSERT_TRUE(otherFileExistenceAfter);
        ASSERT_NE(otherFileExistenceBefore, otherFileExistenceAfter);
    }
} // namespace be::tests::integration
