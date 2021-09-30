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
} // namespace be::tests::integration
