#pragma once
#include "Beast/Graphics/Components.h"

#include "Beast/Common/Types.h"
#include "Beast/Common/Helpers.h"

namespace be::graphics
{
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

        virtual VertexBuffer CreateVertexBuffer() = 0;

        virtual void Clear(const Color& color) const noexcept = 0;
        virtual void Present() const noexcept = 0;
    };
}
