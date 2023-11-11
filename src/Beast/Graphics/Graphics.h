#pragma once
#include "Beast/Graphics/Types.h"

#include "Beast/Common/Types.h"
#include "Beast/Common/Helpers.h"
#include "Beast/Common/Filesystem/Types.h"

namespace be::graphics
{
    class IContext
    {
    public:
        IContext() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IContext);
        virtual ~IContext() = default;

        virtual void Clear(const Color& color) const noexcept = 0;
        virtual void Present() const noexcept = 0;

        // TODO: Remove!
        virtual void Run() = 0;
    };

    class IDevice
    {
    public:
        IDevice() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IDevice);
        virtual ~IDevice() = default;

        virtual VertexBuffer CreateVertexBuffer() = 0;
        virtual VertexShader CreateVertexShader(const FilesystemPath& filepath, const InputLayout& inputLayout) = 0;
        virtual PixelShader CreatePixelShader(const FilesystemPath& filepath) = 0;

        // TODO: Remove!
        virtual void Run() = 0;
    };

    class Graphics final
    {
    public:
        Graphics(Unique<IContext> context, Unique<IDevice> device);

        IContext& Context();
        IDevice& Device();

    private:
        Unique<IContext> m_context;
        Unique<IDevice> m_device;
    };
} // namespace be::graphics
