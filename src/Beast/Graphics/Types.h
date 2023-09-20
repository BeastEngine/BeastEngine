#pragma once

namespace be::graphics
{
    struct Color
    {
        const float* Data() const
        {
            return &r;
        }

        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;
    };
}
