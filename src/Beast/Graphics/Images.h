#pragma once
#include "Beast/Common/Types.h"

#include <vector>

namespace be::graphics
{
    struct Image
    {
        std::vector<unsigned char> data;
        uint32 width = 0;
        uint32 height = 0;
        uint8 numberOfChannels = 0;
    };
} // namespace be::graphics