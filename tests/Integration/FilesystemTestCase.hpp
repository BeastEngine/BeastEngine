#pragma once
#include <TestSetup.hpp>

#include <Beast/Common/Filesystem/Types.hpp>
#include <Beast/Common/Types.hpp>
#include <Beast/Common/UUIDGenerator.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <execution>

namespace be::tests::integration
{
    namespace fs = std::filesystem;

    class FilesystemTestCase : public testing::Test
    {
    public:
        static void SetUpTestSuite()
        {
            m_globalBaseDirectoryPath = fs::temp_directory_path() / "BeTemp" / GenerateRandomString();
            fs::create_directories(m_globalBaseDirectoryPath);
        }

        static void TearDownTestSuite()
        {
            if (fs::exists(m_globalBaseDirectoryPath))
            {
                fs::remove_all(m_globalBaseDirectoryPath);
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

        FilesystemPath CreateDirectory(const std::string& relativePath) const
        {
            auto fullPath = GetFullPath(relativePath);
            fs::create_directory(fullPath);

            return fullPath;
        }

        FilesystemPath CreateDirectory(const FilesystemPath& relativePath) const
        {
            return CreateDirectory(relativePath.u8string());
        }

        FilesystemPath CreateDirectory(const char* relativePath) const
        {
            return CreateDirectory(std::string(relativePath));
        }

        FilesystemPath CreateFile(const std::string& relativePath, const std::string& fileContent = "") const
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

        FilesystemPath CreateFile(const FilesystemPath& relativePath, const std::string& fileContent = "") const
        {
            return CreateFile(relativePath.u8string(), fileContent);
        }

        FilesystemPath CreateFile(const char* relativePath, const std::string& fileContent = "") const
        {
            return CreateFile(std::string(relativePath), fileContent);
        }

        FilesystemPath GetFullPath(const std::string& relativePath) const
        {
            return GetBaseDirectoryPath() + "/" + relativePath;
        }

        FilesystemPath GetFullPath(const FilesystemPath& relativePath) const
        {
            return GetFullPath(relativePath.u8string());
        }

        FilesystemPath GetFullPath(const char* relativePath) const
        {
            return GetFullPath(std::string(relativePath));
        }

        std::string ReadFile(const FilesystemPath& filePath) const
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

        FilesystemPath GetBaseDirectoryPath() const
        {
            return m_testCaseBaseDirPath;
        }

    private:
        void CreateBaseDirectory() const
        {
            fs::create_directories(GetBaseDirectoryPath());
        }

        void DeleteBaseDirectory() const
        {
            const auto baseDirPath = GetBaseDirectoryPath();
            if (fs::exists(baseDirPath))
            {
                fs::remove_all(baseDirPath);
            }
        }

        static std::string GenerateRandomString()
        {
            return GenerateUUID4().ToString();
        }

    private:
        static inline FilesystemPath m_globalBaseDirectoryPath = "";

        FilesystemPath m_testCaseBaseDirPath;
    };
} // namespace be::tests::integration
