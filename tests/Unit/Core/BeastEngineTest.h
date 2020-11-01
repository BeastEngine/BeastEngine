#pragma once
#include "TestSetup.h"

#include <BeastEngine/Core/Windows/IWindowFactory.h>
#include <BeastEngine/Core/Windows/AWindow.h>

namespace be::tests::unit
{
    class WindowMock : public AWindow
    {
    public:
        MOCK_METHOD(void, ProcessInput, (), (override));
    };

    class WindowFactoryMock : public IWindowFactory
    {
    public:
        UniquePtr<IWindow> Create(const WindowDescriptor& descriptor) override
        {
            return UniquePtr<IWindow>(CreateProxy());
        }

        MOCK_METHOD(IWindow*, CreateProxy, ());
    };

    class BeastEngineTest : public ::testing::Test
    {
    protected:
        const be::WindowHandleInstanceType m_wHInstance = nullptr;
    };
} // namespace be::tests::unit
