#pragma once
#include <TestSetup.h>

#include <Beast/Common/Filesystem/Types.h>
#include <Beast/Common/Types.h>
#include <Beast/Common/UUIDGenerator.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <execution>

namespace be::tests::integration
{
    class FilesystemTestCase : public testing::Test
    {
    public:
        static void SetUpTestSuite()
        {
            m_globalBaseDirectoryPath = std::filesystem::temp_directory_path() / "BeTemp" / GenerateRandomString();
            std::filesystem::create_directories(m_globalBaseDirectoryPath);
        }

        static void TearDownTestSuite()
        {
            if (std::filesystem::exists(m_globalBaseDirectoryPath))
            {
                std::filesystem::remove_all(m_globalBaseDirectoryPath);
            }
        }

        FilesystemTestCase()
        {
            m_testCaseBaseDirPath = m_globalBaseDirectoryPath / GenerateRandomString();
        }

    protected:
        void SetUp() override
        {
            CreateBaseDirectory();
        }

        void TearDown() override
        {
            DeleteBaseDirectory();
        }

        fs::Path CreateDirectory(const std::string& relativePath) const
        {
            return CreateDirectoryImpl(relativePath);
        }

        fs::Path CreateDirectory(const fs::Path& relativePath) const
        {
            return CreateDirectoryImpl(relativePath.u8string());
        }

        fs::Path CreateDirectory(const char* relativePath) const
        {
            return CreateDirectoryImpl(std::string(relativePath));
        }

        fs::Path CreateFile(const std::string& relativePath, const std::string& fileContent = "") const
        {
            return CreateFileImpl(relativePath, fileContent);
        }

        fs::Path CreateFile(const fs::Path& relativePath, const std::string& fileContent = "") const
        {
            return CreateFileImpl(relativePath.u8string(), fileContent);
        }

        fs::Path CreateFile(const char* relativePath, const std::string& fileContent = "") const
        {
            return CreateFileImpl(std::string(relativePath), fileContent);
        }

        fs::Path GetFullPath(const std::string& relativePath) const
        {
            return GetFullPathImpl(relativePath);
        }

        fs::Path GetFullPath(const fs::Path& relativePath) const
        {
            return GetFullPathImpl(relativePath.u8string());
        }

        fs::Path GetFullPath(const char* relativePath) const
        {
            return GetFullPathImpl(std::string(relativePath));
        }

        std::string ReadFile(const fs::Path& filePath) const
        {
            std::ifstream file(filePath);
            if (!file.is_open())
            {
                throw std::runtime_error("Could not open file");
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();

            return buffer.str();
        }

        fs::Path GetBaseDirectoryPath() const
        {
            return m_testCaseBaseDirPath;
        }

    private:
        fs::Path GetFullPathImpl(const auto& relativePath) const
        {
            return GetBaseDirectoryPath() / relativePath;
        }

        fs::Path CreateFileImpl(const auto& relativePath, const std::string& fileContent = "") const
        {
            auto fullFilePath = GetFullPath(relativePath);
            std::ofstream file(fullFilePath);
            if (!fileContent.empty())
            {
                file << fileContent;
            }
            file.close();

            return fullFilePath;
        }

        fs::Path CreateDirectoryImpl(const auto& relativePath) const
        {
            auto fullPath = GetFullPath(relativePath);
            std::filesystem::create_directory(fullPath);

            return fullPath;
        }

        void CreateBaseDirectory() const
        {
            std::filesystem::create_directories(GetBaseDirectoryPath());
        }

        void DeleteBaseDirectory() const
        {
            const auto baseDirPath = GetBaseDirectoryPath();
            if (std::filesystem::exists(baseDirPath))
            {
                std::filesystem::remove_all(baseDirPath);
            }
        }

        static std::string GenerateRandomString()
        {
            return GenerateUUID4().ToString();
        }

    private:
        static inline fs::Path m_globalBaseDirectoryPath = "";

        fs::Path m_testCaseBaseDirPath;
    };
} // namespace be::tests::integration
