#include "Beast/Graphics/GraphicsFactory.h"
#include "Beast/Common/Exceptions.h"

#ifdef BE_PLATFORM_WINDOWS
    #include "Beast/Graphics/D3D11/Device.h"
    #include "Beast/Graphics/D3D11/Context.h"
#endif

namespace be::internals
{
    Unique<graphics::Graphics> CreateGraphics(graphics::RenderingApi api, const IWindow& window)
    {
        switch (api)
        {
        case be::graphics::RenderingApi::D3D11:
#ifdef BE_PLATFORM_WINDOWS
        {
            auto device = MakeUnique<graphics::d3d11::Device>(window);
            auto context = MakeUnique<graphics::d3d11::Context>(*device);

            return MakeUnique<graphics::Graphics>(std::move(device), std::move(context));
        }

        break;
#else
            BE_THROW("Platform not supported! Could not create a graphics adapter!");
#endif
        default:
            break;
        }

        BE_THROW("Given API not supported!");
    }
} // namespace be::graphics::internals
