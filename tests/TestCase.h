#pragma once
#include "TestSetup.h"

#include <filesystem>
#include <codecvt>
#include <locale>

namespace be::tests
{
    class TestCase : public ::testing::Test
    {
    public:
        inline static std::string TempFilePath()
        {
            return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(std::filesystem::temp_directory_path().c_str());
        }
    };
} // namespace be::tests
