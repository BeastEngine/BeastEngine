#include "Beast/Graphics/GraphicsFactory.h"
#include "Beast/Common/Exceptions.h"

#ifdef BE_PLATFORM_WINDOWS
    #include "Beast/Graphics/D3D11/Device.h"
    #include "Beast/Graphics/D3D11/Context.h"
#endif

namespace be::graphics
{
    Unique<Graphics> DefaultGraphicsFactory::Create(RenderingApi api, const IWindow& window) const
    {
        switch (api)
        {
        case be::graphics::RenderingApi::D3D11:
#ifdef BE_PLATFORM_WINDOWS
        {
            auto device = MakeUnique<d3d11::Device>(window);
            auto context = MakeUnique<d3d11::Context>(*device);

            return MakeUnique<Graphics>(std::move(context), std::move(device));
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
