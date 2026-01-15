#pragma once
#include "Beast/Common/Filesystem/Types.h"
#include "Beast/Common/Result.h"
#include "Beast/Common/Id.h"

#include <unordered_map>

namespace be::fs
{
    class ResourcesManager final
    {
    public:
        /**
         * @brief Scans given directory searching for files in the following special folders:
         *  - Textures|textures - All files found in this path will have their id made out of "Textures/rest_of_the_path_to_the_file".
         * 
         * @param resourcesRootDir 
         */
        explicit ResourcesManager(const Path& resourcesRootDir);

        /**
         * @brief Returns absolute path to the resource identified by the given \p resourceId.
         * The ids for resources are automatically generated when the manager is created. Each id is built out of filepath relative to the \c resourcesRootDir.
         * Eg. "Textures/MyTexture.png". This means, that user can provide a string Id with the path to the resource they are interested in.
         * 
         * @param resourceId 
         * @return RefResult<Path> 
         */
        RefResult<Path> GetResourcePath(const Id& resourceId) const;

    private:
        std::unordered_map<Id, Path, Id::Hasher> m_paths;
    };
} // namespace be::fs