#pragma once
#include <filesystem>

namespace be::fs
{
    using Path = std::filesystem::path;

    inline Path operator+(const Path& path1, const Path& path2)
    {
        return Path(path1.u8string() + path2.u8string());
    }
} // namespace be::fs
