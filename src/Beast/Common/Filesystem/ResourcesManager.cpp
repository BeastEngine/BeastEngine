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
                        const auto textureId = graphics::TextureId(textureFilePath.generic_string());

                        m_paths[textureId.Raw()] = std::filesystem::absolute(texture_entry.path());
                    }
                }
            }
        }
    }

    // TODO: This shouldn't probably depend on graphics and TextureId. I believe we can instead just use be::Id for this.
    // Or maybe create a new "StringId" type for that kind of stuff.
    RefResult<Path> ResourcesManager::GetResourcePath(graphics::TextureId textureId) const
    {
        const auto foundPath = m_paths.find(textureId.Raw());
        if (foundPath == m_paths.end())
        {
            return std::format("Texture with id {} does not exist", textureId.Raw());
        }

        return foundPath->second;
    }
} // namespace be::fs