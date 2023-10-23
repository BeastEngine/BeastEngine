#pragma once
#include "Beast/Graphics/Components.h"

#include "Beast/Common/Types.h"
#include "Beast/Common/Helpers.h"

namespace be::graphics
{
    enum class RenderingApi
    {
        D3D11,
    };

    struct VertexBuffer
    {
        const Id id;
    };

    class IContext
    {
    public:
        IContext() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IContext);
        virtual ~IContext() = default;

        virtual void Clear(const Color& color) const noexcept = 0;
        virtual void Present() const noexcept = 0;
    };

    class IDevice
    {
    public:
        IDevice() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IDevice);
        virtual ~IDevice() = default;

        virtual VertexBuffer CreateVertexBuffer() = 0;
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
