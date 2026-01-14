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
        explicit ResourcesManager(const Path& resourcesRootDir);
        RefResult<Path> GetResourcePath(const Id& resourceId) const;

    private:
        std::unordered_map<Id, Path, Id::Hasher> m_paths;
    };
} // namespace be::fs