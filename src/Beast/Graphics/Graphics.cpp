#include "Beast/Graphics/Graphics.h"

namespace be::graphics
{
    Graphics::Graphics(Unique<IDevice> device, Unique<IContext> context)
        : m_device(std::move(device)), m_context(std::move(context))
    {
    }

    IContext& Graphics::Context()
    {
        return *m_context;
    }

    IDevice& Graphics::Device()
    {
        return *m_device;
    }
} // namespace be::graphics
