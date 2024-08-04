#pragma once
#include "Beast/Common/Types.h"
#include "Beast/Common/Id.h"

#include "Beast/Math/Types.h"

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

    struct Vertex
    {
        Vec2 position;
        Color color;
        Vec2 uv;
    };

    constexpr inline auto VERTEX_STRIDE = sizeof(Vertex);

    enum class RenderingApi
    {
        D3D11,
    };

    struct VertexBuffer
    {
        Id id;
    };

    struct InputLayout
    {
        struct VertexAttribute
        {
            enum class Format
            {
                Vec2,
                Vec4,
            };

            const char* name;
            Format format;
            uint32 index = 0u;
        };

        std::vector<VertexAttribute> vertexAttributes;
    };

    struct VertexShader
    {
        Id id;
    };

    struct PixelShader
    {
        Id id;
    };
} // namespace be::graphics
