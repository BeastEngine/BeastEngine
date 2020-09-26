#pragma once
#include "TestSetup.h"

#include <spdlog/logger.h>
#include <BeastEngine/Core/Logging.h>

#include <filesystem>
#include <codecvt>
#include <locale>


namespace be::tests::integration
{
    struct LoggersFactoryTestParams
    {
        LoggerType loggerType;
        std::string additionalParams = "";
    };

    class LoggersFactoryTest : public ::testing::TestWithParam<LoggersFactoryTestParams>
    {
    public:
        inline static std::string TempFilePath()
        {
            return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(std::filesystem::temp_directory_path().c_str());
        }
    protected:
    };
} // namespace be::tests::unit
