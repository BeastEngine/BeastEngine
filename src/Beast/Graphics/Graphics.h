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

    class IRenderContext
    {
    public:
        IRenderContext() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IRenderContext);
        virtual ~IRenderContext() = default;

        virtual void Draw(const Pipeline& pipeline) const= 0;
        virtual void Clear(const Color& color) const= 0;
        virtual void Present() const = 0;

        virtual void UpdateVertexBuffer(VertexBuffer buffer, std::span<const Vertex> verticies) const = 0;
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

        virtual const IRenderContext& GetContext() const noexcept = 0;
        
        // TODO: Remove!
        virtual void Run() = 0;
    };

    /*class Graphics final
    {
    public:
        Graphics(RenderingApi api, be::IWindow& window);
        ~Graphics();

        IRenderContext& Context();
        IGraphics& Device();

    private:
        Unique<IGraphics> m_device;
        Unique<IRenderContext> m_context;
    };*/
} // namespace be::graphics
