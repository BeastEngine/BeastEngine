#pragma once
#include "Beast/Common/Types.h"
#include "Beast/Math/Types.h"

namespace be
{
    namespace graphics
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

        struct Vertex
        {
            Vec2 position;
            Color color;
            Vec2 uv;
        };
    } // namespace graphics

    enum class Layer : uint8
    {
        Back = 0,
        Middle = 1,
        Front = 2,
    };

    constexpr auto LAYERS_COUNT = 3;

    struct Texture
    {
        Id id = ID_EMPTY;
        Vec2 uvCoords = {0.0f, 0.0f};
    };

    struct Material
    {
        graphics::Color color;
    };

    struct Sprite
    {
        Texture texture;
        Material material;
        Layer layer = Layer::Middle;
    };
} // namespace be
