#pragma once
#include <Beast/Windows/IWindowFactory.h>

#include <Beast/Common/Helpers.h>

namespace be::internals
{
    /**
     * @brief Basic window factory.
     * Creates Win32Window for Windows.
     * Throws exception if current platform is not supported.
     */
    class WindowFactory final : public IWindowFactory
    {
        friend class BeastEngine;

    public:
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(WindowFactory)

        WindowFactory() = default;
        ~WindowFactory() = default;

    private:
        Unique<IWindow> Create(const WindowDescriptor& descriptor) override;
    };
} // namespace be::internals
