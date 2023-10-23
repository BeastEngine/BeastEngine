#pragma once
#include <TestSetup.h>

#include <Beast/Windows/IWindowFactory.h>
#include <Beast/Windows/AWindow.h>

namespace be::tests::unit
{
    class WindowMock : public AWindow
    {
    public:
        MOCK_METHOD(void, ProcessInput, (), (override));
        MOCK_METHOD(WindowHandle, GetHandle, (), (const, noexcept, override));
    };

    class WindowFactoryMock : public IWindowFactory
    {
    public:
        MOCK_METHOD(Unique<IWindow>, Create, (const WindowDescriptor&), (override));
    };

    class BeastEngineTest : public testing::Test
    {
    protected:
        auto CreateWindowFactoryMock()
        {
            return MakeUnique<WindowFactoryMock>();
        }

        auto CreateWindowMock()
        {
            return MakeUnique<WindowMock>();
        }

    protected:
        const be::WindowHandleInstanceType m_wHInstance = nullptr;
    };
} // namespace be::tests::unit
