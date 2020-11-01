#pragma once
#include "IWindowFactory.h"
#include "BeastEngine/Core/Helpers.h"

namespace be::internals
{
    class WindowFactory final : public IWindowFactory
    {
        friend class BeastEngine;

    public:
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(WindowFactory)

        WindowFactory() = default;
        ~WindowFactory() = default;

    private:
        UniquePtr<IWindow> Create(const WindowDescriptor& descriptor) override;
    };
} // namespace be::internals
