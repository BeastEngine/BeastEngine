#include "Beast/Common/Filesystem/ResourcesManager.h"

#include <format>

namespace be::fs
{
    ResourcesManager::ResourcesManager(const Path& resourcesRootDir)
    {
        for (const auto& directory : std::filesystem::directory_iterator(resourcesRootDir))
        {
            if (!directory.is_directory())
            {
                continue;
            }

            const auto& directoryName = directory.path().filename().string();
            if (directoryName == "Textures" || directoryName == "textures")
            {
                for (const auto& texture_entry : std::filesystem::recursive_directory_iterator(directory.path()))
                {
                    if (texture_entry.is_regular_file())
                    {
                        const auto textureFilePath = std::filesystem::relative(texture_entry.path(), directory.path());
                        const auto textureId = Id(textureFilePath.generic_string());

                        m_paths[textureId] = std::filesystem::absolute(texture_entry.path());
                    }
                }
            }
        }
    }

    RefResult<Path> ResourcesManager::GetResourcePath(const Id& resourceId) const
    {
        const auto foundPath = m_paths.find(resourceId);
        if (foundPath == m_paths.end())
        {
            return std::format("Resource with id {} does not exist", resourceId.Raw());
        }

        return foundPath->second;
    }
} // namespace be::fs