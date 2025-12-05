#pragma once
#include "Beast/Graphics/Types.h"

#include "Beast/Common/Types.h"
#include "Beast/Common/Helpers.h"
#include "Beast/Common/Filesystem/Types.h"

namespace be
{
    class Window;
}

namespace be::graphics
{
    struct Image;
    struct Pipeline;

    class IGraphics
    {
    public:
        IGraphics() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IGraphics);
        virtual ~IGraphics() = default;

        virtual VertexBuffer CreateVertexBuffer(uint32 stride, uint32 maxSize) = 0;
        virtual ConstantBuffer CreateConstantBuffer(uint32 size) = 0;
        virtual VertexShader CreateVertexShader(const fs::Path& filepath, const InputLayout& inputLayout) = 0;
        virtual PixelShader CreatePixelShader(const fs::Path& filepath) = 0;
        virtual Texture CreateTexture(const Image& textureData) = 0;

        virtual void UpdateVertexBuffer(VertexBuffer buffer, std::span<const Vertex> verticies) = 0;
        virtual void UpdateConstantBuffer(ConstantBuffer buffer, const void* const data) = 0;

        virtual void Draw(const Pipeline& pipeline) = 0;
        virtual void Clear(const Color& color) = 0;
        virtual void Present() = 0;
    };
} // namespace be::graphics
