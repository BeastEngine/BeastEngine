#pragma once
#include "Beast/Graphics/Graphics.h"

#include "Beast/Windows/IWindow.h"
#include "Beast/Common/Helpers.h"
#include "Beast/Common/Types.h"

namespace be::graphics
{
    class IGraphicsFactory
    {
    public:
        IGraphicsFactory() = default;
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IGraphicsFactory);
        virtual ~IGraphicsFactory() = default;

        virtual Unique<Graphics> Create(RenderingApi api, const IWindow& window) const = 0;
    };

    // TODO: This should be part of the "internals" namespace
        class DefaultGraphicsFactory final : public IGraphicsFactory
        {
        public:
            Unique<Graphics> Create(RenderingApi api, const IWindow& window) const override;
        };

} // namespace be::graphics
