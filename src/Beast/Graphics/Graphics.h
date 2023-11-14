#pragma once
#include "Beast/Graphics/Types.h"

#include "Beast/Common/Types.h"
#include "Beast/Common/Helpers.h"
#include "Beast/Common/Filesystem/Types.h"

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

    class IContext
    {
    public:
        IContext() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IContext);
        virtual ~IContext() = default;

        virtual void Draw(const Pipeline& pipeline) const noexcept = 0;
        virtual void Clear(const Color& color) const noexcept = 0;
        virtual void Present() const noexcept = 0;

        virtual void UpdateVertexBuffer(VertexBuffer buffer, std::span<const Vertex> verticies) = 0;
    };

    class IDevice
    {
    public:
        IDevice() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IDevice);
        virtual ~IDevice() = default;

        virtual VertexBuffer CreateVertexBuffer(uint32 stride, uint32 maxSize) = 0;
        virtual VertexShader CreateVertexShader(const FilesystemPath& filepath, const InputLayout& inputLayout) = 0;
        virtual PixelShader CreatePixelShader(const FilesystemPath& filepath) = 0;

        // TODO: Remove!
        virtual void Run() = 0;
    };

    class Graphics final
    {
    public:
        Graphics(Unique<IDevice> device, Unique<IContext> context);

        IContext& Context();
        IDevice& Device();

    private:
        Unique<IDevice> m_device;
        Unique<IContext> m_context;
    };
} // namespace be::graphics
