#pragma once
#include <Beast/Graphics/Types.h>
#include <Beast/Common/Helpers.h>

namespace be::graphics
{
    class IContext
    {
    public:
        IContext() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IContext);
        virtual ~IContext() = default;

        virtual void ClearRenderTargetView(const Color& color) const noexcept = 0;
        virtual void Present() const noexcept = 0;
    };
}
