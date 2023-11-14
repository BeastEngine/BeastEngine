#pragma once
#include "Beast/Math/Types.h"

namespace be
{
    struct Transform
    {
        Vec2 position = {0.0f, 0.0f};

        bool operator==(const Transform& other) const
        {
            return position == other.position;
        }

        bool operator!=(const Transform& other) const
        {
            return position != other.position;
        }
    };
} // namespace be
