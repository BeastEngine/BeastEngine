#pragma once
#include "Beast/Common/Types.h"
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
    };

    enum class RenderingApi
    {
        D3D11,
    };

    struct VertexBuffer
    {
        const Id id;
    };

    struct InputLayout
    {
        struct VertexAttribute
        {
            enum class Format
            {
                Float2,
                Float4,
            };

            const char* name;
            Format format;
            uint32 index = 0u;
        };

        std::vector<VertexAttribute> vertexAttributes;
    };

    struct VertexShader
    {
        const Id id;
    };

    struct PixelShader
    {
        const Id id;
    };
}
