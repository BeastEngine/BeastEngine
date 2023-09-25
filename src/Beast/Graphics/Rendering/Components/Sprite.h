#pragma once
#include <Beast/Graphics/Types.h>

#include <Beast/Common/Types.h>
#include <Beast/Math/Types.h>

namespace be::graphics
{
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