#pragma once
#include "Beast/Graphics/Types.h"

#include "Beast/Common/Types.h"
#include "Beast/Common/Helpers.h"
#include "Beast/Common/Filesystem/Types.h"

namespace be
{
    class IWindow;
}

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
        };

        struct Viewport
        {
            Vec2 dimensions;
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

        DrawCall drawCall;
    };

    class IGraphics
    {
    public:
        IGraphics() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IGraphics);
        virtual ~IGraphics() = default;

        virtual VertexBuffer CreateVertexBuffer(uint32 stride, uint32 maxSize) = 0;
        virtual VertexShader CreateVertexShader(const FilesystemPath& filepath, const InputLayout& inputLayout) = 0;
        virtual PixelShader CreatePixelShader(const FilesystemPath& filepath) = 0;

        virtual void UpdateVertexBuffer(VertexBuffer buffer, std::span<const Vertex> verticies) = 0;

        virtual void Draw(const Pipeline& pipeline) = 0;
        virtual void Clear(const Color& color) = 0;
        virtual void Present() = 0;
    };
} // namespace be::graphics
