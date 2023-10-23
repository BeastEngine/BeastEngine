#include "Beast/Graphics/Graphics.h"

namespace be::graphics
{
    Graphics::Graphics(Unique<IContext> context, Unique<IDevice> device)
        : m_context(std::move(context)),
          m_device(std::move(device))
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
