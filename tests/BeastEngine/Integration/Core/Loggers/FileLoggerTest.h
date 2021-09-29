#pragma once
#include "BeastEngine/Integration/FilesystemTestCase.h"

#include <BeastEngine/Core/Loggers/LoggersFactories.h>
#include <set>

namespace be::tests::integration
{
    class FileLoggerTest : public FilesystemTestCase
    {
    protected:
        void TearDown() override
        {
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
} // namespace be::tests::integration
