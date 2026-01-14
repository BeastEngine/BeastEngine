#pragma once
#include "Beast/Common/Filesystem/Types.h"
#include "Beast/Graphics/Types.h"
#include "Beast/Common/Result.h"

#include <unordered_map>

namespace be::fs
{
    class ResourcesManager final
    {
    public:
        explicit ResourcesManager(const Path& resourcesRootDir);
        RefResult<Path> GetResourcePath(graphics::TextureId textureId) const;

    private:
        std::unordered_map<be::RawId, Path> m_paths;
    };
} // namespace be::fs