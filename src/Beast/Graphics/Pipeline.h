#pragma once
#include "Beast/Graphics/Types.h"

namespace be::graphics
{
    struct Pipeline
    {
        struct VSStage
        {
            VertexShader shader;
        };

        struct PSStage
        {
            PixelShader shader;
            Texture texture;
        };

        struct DrawCall
        {
            uint32 firstVertexIndex = 0;
            uint32 vertexCount = 3;
        };

        VertexBuffer vertexBuffer;
        VSStage vertexShaderStage;
        PSStage pixelShaderStage;
        Viewport viewport;

        ConstantBuffer constantBuffer;

        DrawCall drawCall;
    };
} // namespace be::graphics