#include "Beast/Graphics/GraphicsFactory.h"
#include "Beast/Debug.h"

#ifdef BE_PLATFORM_WINDOWS
    #include "Beast/Graphics/D3D11/Graphics.h"
#endif

namespace be::internals
{
    Unique<graphics::IGraphics> CreateGraphics(graphics::RenderingApi api, const IWindow& window)
    {
        switch (api)
        {
#ifdef BE_PLATFORM_WINDOWS
        case be::graphics::RenderingApi::D3D11:
            return MakeUnique<graphics::d3d11::Graphics>(window);
#endif
        }

        BE_THROW("API not supported! Could not create the graphics adapter!");
    }
} // namespace be::internals
