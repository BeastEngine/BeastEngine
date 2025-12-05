#pragma once
#include "Beast/Graphics/Images.h"

#include "Beast/Common/Types.h"
#include "Beast/Common//Filesystem/Types.h"
#include "Beast/Common/Result.h"

namespace be::graphics
{
    Result<Image> LoadImageFromFile(const fs::Path& imagePath);
} // namespace be::graphics
