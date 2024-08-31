#pragma once
#include <filesystem>

namespace be
{
    using FilesystemPath = std::filesystem::path;

    inline FilesystemPath operator+(const FilesystemPath& path1, const FilesystemPath& path2)
    {
        return FilesystemPath(path1.u8string() + path2.u8string());
    }
} // namespace be
